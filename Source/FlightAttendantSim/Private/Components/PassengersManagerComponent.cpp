// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PassengersManagerComponent.h"
#include "Characters/FABasePassenger.h"
#include "Characters/Passengers/DocsInfo.h"
#include "DocsInfoStruct.h"
#include "Kismet/GameplayStatics.h"
#include "WorldMap/LocationInfo.h"


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
	UWorld* World = GetWorld();
	if (World)
	{
		AFABasePassenger* Passenger = World->SpawnActorDeferred<AFABasePassenger>(
			PassengerToSpawn,
			PassengerTransform
		);
		if (Passenger)
		{
			Passenger->SetDocsInfo(CreateDocument());
			Passenger->SetDestinationLocation(Destination);
			UGameplayStatics::FinishSpawningActor(Passenger, Passenger->GetActorTransform());
			SpawnedPassengers.Add(Passenger);
		}
		return Passenger;
	}

	return nullptr;
}

void UPassengersManagerComponent::ClearPassengers()
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
}

FDocsInfo UPassengersManagerComponent::CreateDocument() const
{
	return
	{
		LastNames[FMath::RandHelper(LastNames.Num())],
		FirstNames[FMath::RandHelper(FirstNames.Num())],
		FDocsInfo::GenerateId()
	};
}

