// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PassengersManagerComponent.h"

#include "Characters/FABasePassenger.h"
#include "Characters/Passengers/DocsInfo.h"
#include "DocsInfoStruct.h"
#include "Flight/FlightControlComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FAGameMode.h"
#include "Components/WaypointsComponent.h"
#include "Components/GameEconomyComponent.h"
#include "SpacePlane/SpacePlaneComponent.h"
#include "Components/PDAMessengerComponent.h"
#include "WorldMap/LocationInfo.h"
#include "Characters/ActionExecutorComponent.h"
#include "Characters/Actions/CharacterMoveAction.h"
#include "Characters/Actions/CharacterStandAction.h"

// Sets default values for this component's properties
UPassengersManagerComponent::UPassengersManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

AFABasePassenger* UPassengersManagerComponent::CreatePassenger(const FTransform& PassengerTransform, ULocationInfo* Destination)
{
	return SpawnPassenger({
		PassengerTransform,
		Destination,
		CreateDocument()
	});
}

AFABasePassenger* UPassengersManagerComponent::PopBufferedPassenger()
{
	if (BufferedPassengers.Num() > 0)
	{
		const FPassengerSpawnParams SpawnParams = BufferedPassengers.Pop(false);
		return SpawnPassenger(SpawnParams);
	}
	else
	{
		BufferedPassengers.Shrink();
		return nullptr;
	}
}

void UPassengersManagerComponent::ClearRedundantPassengers()
{
	TArray<int32> DestroyedActors;
	for (int32 i = 0; i < SpawnedPassengers.Num(); ++i)
	{
		if (SpawnedPassengers[i]->GetAssignedPassengerSeat() == nullptr)
		{
			SpawnedPassengers[i]->Destroy();
			DestroyedActors.Add(i);
		}
	}
	for (int32 i = DestroyedActors.Num() - 1; i >= 0; --i)
	{
		SpawnedPassengers.RemoveAt(DestroyedActors[i], 1, false);
	}
	SpawnedPassengers.Shrink();
}

// Called when the game starts
void UPassengersManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	LastNames = UDocsInfoStruct::LoadUsingFileHelperStrings(TCHAR_TO_UTF8(*PathToLastNames));
	FirstNames = UDocsInfoStruct::LoadUsingFileHelperStrings(TCHAR_TO_UTF8(*PathToFirstNames));

	AFAGameMode* GameMode = FAGAMEMODE;
	GameMode->LocationLoaded.AddDynamic(this, &UPassengersManagerComponent::LocationLoadedHandle);
	UFlightControlComponent* FlightControl = GameMode->GetFlightController();
	FlightControl->PlayerArrived.AddDynamic(this, &UPassengersManagerComponent::PlayerArrivedHandle);
}

AFABasePassenger* UPassengersManagerComponent::SpawnPassenger(const FPassengerSpawnParams& SpawnParams)
{
	UWorld* World = GetWorld();
	if (World)
	{
		AFABasePassenger* Passenger = World->SpawnActorDeferred<AFABasePassenger>(
			PassengerToSpawn,
			SpawnParams.PassengerTransform
		);
		if (Passenger)
		{
			Passenger->SetDocsInfo(SpawnParams.DocumentsInfo);
			Passenger->SetDestinationLocation(SpawnParams.LocationHeadingTo.Get());
			UGameplayStatics::FinishSpawningActor(Passenger, Passenger->GetActorTransform());
			SpawnedPassengers.Add(Passenger);
		}
		return Passenger;
	}

	return nullptr;
}

void UPassengersManagerComponent::LocationLoadedHandle(const UMapNode* Destination)
{
	// Characters must be placed on scene at this point
	if (WantedCharacters.Num() == 0)
		return;
	
	FPDAMessage Message;
	Message.Author = {
		"Security",
		FText::FromString("Transportation Security Agency")
	};
	{
		FString MessageString = SuspendMessageTemplate.ToString();
		for (const FDocsInfo& Doc : WantedCharacters)
		{
			MessageString.Append("\n");
			MessageString.Append(Doc.PassengerId);
		}
		Message.Message = FText::FromString(MessageString);
	}
	UPDAMessengerComponent* PDAMessenger = FAGAMEMODE->GetPDAMessenger();
	PDAMessenger->AddScheduledMessage(Message, 15.0f);
}

void UPassengersManagerComponent::PlayerArrivedHandle(UMapNode* Destination)
{
	ExecutePenalties();
	WantedCharacters.Empty();
	PassengersLeavePlane(Destination);
}

void UPassengersManagerComponent::ExecutePenalties() const
{
	const AFAGameMode* GameMode = FAGAMEMODE;
	int64 PenaltySum = 0; 
	for (const AFABasePassenger* Passenger : SpawnedPassengers)
	{
		if (Passenger->IsAssigned() && Passenger->GetDocsInfo().IsWanted)
		{
			if (FMath::FRand() <= WantedCharacterTransportationPenaltyRate)
			{
				PenaltySum += WantedCharacterTransportationPenalty;
			}
		}
	}
	if (PenaltySum > 0)
	{
		const float Delay = FMath::RandRange(
			WantedCharacterTransportationDelayMin,
			WantedCharacterTransportationDelayMax);
		GameMode->GetEconomyComponent()->WithdrawPlayerMoneyScheduled(PenaltySum, Delay);
		FPDAMessage Message;
		Message.Author = {
			"Security",
			FText::FromString("Transportation Security Agency")
		};
		Message.Message = FText::FromString(FString::Printf(
			TEXT("You have been fined $%lld for transporting persons prohibited from travelling."),
			PenaltySum));
		GameMode->GetPDAMessenger()->AddScheduledMessage(Message, Delay);
	}
}

void UPassengersManagerComponent::PassengersLeavePlane(const UMapNode* Destination)
{
	const AFAGameMode* GameMode = FAGAMEMODE;
	const UWaypointsComponent* Waypoints = GameMode->GetWaypointsComponent();
	USpacePlaneComponent* SpacePlane = GameMode->GetSpacePlane();
	int64 TransitPayment = 0;
	FTransform LeavePlaneWaypointTransform;
	Waypoints->GetWaypointByName(LeavePlaneWaypoint, LeavePlaneWaypointTransform);
	for (AFABasePassenger* Passenger : SpawnedPassengers)
	{
		if (Passenger->IsAssigned() &&
			Passenger->GetLocationHeadingTo()->CorrespondingNode == Destination)
		{
			SpacePlane->DeassignPassenger(Passenger);
			TransitPayment += PassengerTicketPrice;
			const TArray<UCharacterAction*> LeavePlaneActions = {
				UCharacterStandAction::CreateCharacterStandAction(this),
				UCharacterMoveAction::CreateCharacterMoveAction(
					LeavePlaneWaypointTransform.GetLocation(),
					this)
			};
			UActionExecutorComponent* ActionExecutor =
				Passenger->GetActionExecutorComponent();
			ActionExecutor->SetActions(LeavePlaneActions);
			ActionExecutor->ExecuteActions();
		}
	}
	if (TransitPayment > 0)
	{
		UGameEconomyComponent* GameEconomy = FAGAMEMODE->GetEconomyComponent();
		GameEconomy->AddPlayerMoney(TransitPayment);
	}
}

FDocsInfo UPassengersManagerComponent::CreateDocument()
{
	FDocsInfo ResultDoc = {
		LastNames[FMath::RandHelper(LastNames.Num())],
		FirstNames[FMath::RandHelper(FirstNames.Num())],
		FDocsInfo::GenerateId(),
		FMath::FRand() <= WantedCharacterSpawnRate
	};
	if (ResultDoc.IsWanted && WantedCharacters.Num() < MaxWantedCharactersNum)
	{
		WantedCharacters.Add(ResultDoc);
	}
	else
	{
		ResultDoc.IsWanted = false;
	}
	return ResultDoc;
}

