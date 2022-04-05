// Fill out your copyright notice in the Description page of Project Settings.


#include "FAGameMode.h"

#include "FlightControlComponent.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Actions/CharacterSitAction.h"
#include "Characters/Actions/CharacterMoveAction.h"
#include "Characters/ActionExecutorComponent.h"
#include "SpacePlane/PassengerSeat.h"
#include "Engine/AssetManager.h"
#include "Quests/Quest.h"
#include "Characters/FABaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SpacePlane/SpacePlaneComponent.h"
#include "Characters/FABasePassenger.h"
#include "Components/PassengersManagerComponent.h"
#include "Components/CargoManagerComponent.h"
#include "Components/PDAMessengerComponent.h"
#include "Components/GameEconomyComponent.h"
#include "SaveGame/SaveGameComponent.h"

AFAGameMode::AFAGameMode()
{
	PassengersManager =
		CreateDefaultSubobject<UPassengersManagerComponent>("PassengerManagerComponent");
	CargoDeliveryManager =
		CreateDefaultSubobject<UCargoManagerComponent>("CargoDeliveryManagerComponent");
	PDAMessenger =
		CreateDefaultSubobject<UPDAMessengerComponent>("PDAMessengerComponent");
	SpacePlane =
		CreateDefaultSubobject<USpacePlaneComponent>("SpacePlaneComponent");
	EconomyComponent =
		CreateDefaultSubobject<UGameEconomyComponent>("EconomyComponent");
	SaveGameComponent =
		CreateDefaultSubobject<USaveGameComponent>("SaveGameComponent");

	const AFAGameMode* DefaultObject = Cast<AFAGameMode>(GetClass()->GetDefaultObject());
	const TSubclassOf<UFlightControlComponent> DefaultClass = DefaultObject->DefaultFlightControllerClass;
	if (DefaultClass)
	{
		FlightController = Cast<UFlightControlComponent>(
		CreateDefaultSubobject(
			TEXT("FlightControlComponent"),
			UFlightControlComponent::StaticClass(),
			*DefaultClass,
			true,
			false));
	}         
}

void AFAGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	DebugCharactersSpawnLocations.Empty();

	WorldMap = NewObject<UMapGraph>(this);
	
	// Load game
	if (!SaveGameComponent->LoadGame())
	{
		{ // TODO: Decompose to protected func
			const UAssetManager& AssetManager = UAssetManager::Get();
			const FPrimaryAssetType QuestAssetType = TEXT("Quest");
			TArray<FPrimaryAssetId> QuestAssetsIds; 
			AssetManager.GetPrimaryAssetIdList(QuestAssetType, QuestAssetsIds);
			//const int32 QuestsToTake = FMath::RandRange(0, MaxNodeChildren);
			for (int i = 0; i < QuestAssetsIds.Num(); ++i)
			{
				//const int32 RandIndex = FMath::RandHelper(QuestAssetsIds.Num());
				//FPrimaryAssetId SampledAssetId = QuestAssetsIds[RandIndex];
				FAssetData AssetData;
				AssetManager.GetPrimaryAssetData(QuestAssetsIds[i], AssetData);
				const auto Temp = Cast<UBlueprint>(AssetData.GetAsset());
				UQuest* LoadedQuest = NewObject<UQuest>(this, Temp->GeneratedClass);
				if (LoadedQuest)
				{
					AvailableQuests.Add(LoadedQuest);
					//QuestAssetsIds.RemoveAtSwap(RandIndex);
				}
			}
		}
	
		TArray<UQuest*> QuestsToPlace = SampleQuestList(MaxNodeChildren);

		if (WorldMap)
		{
			WorldMap->GenerateMap(WorldMapMaxDepth, QuestsToPlace);
		}
		UpdatePlacedQuests(WorldMap->GetRootNode(), QuestsToPlace);

		// Not sure where to call
		LocationLoadedHandle();
	}
}

void AFAGameMode::BeginPlay()
{
	Super::BeginPlay();

	{
		// SpacePlane is being created in InitGame
		check(SpacePlane);
		TArray<AActor*> PassengerSeats;
		UGameplayStatics::GetAllActorsOfClass(
			GetWorld(), APassengerSeat::StaticClass(), PassengerSeats);
		SpacePlane->SetUp(PassengerSeats);
	}

	//FillPassengers();
	//CargoDeliveryManager->GenerateCargoes(WorldMap->GetCurrentNode());

	PostLoadInitialization();
}

void AFAGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	SaveGameComponent->SaveGame();
}

void AFAGameMode::PostLoadInitialization()
{
	ChangeLocation(WorldMap->GetCurrentNode());
}

void AFAGameMode::LocationLoadedHandle()
{
	// TODO: In fact I also need to init their structure at the moment they got taken by Player.
	// Updating all quests structure.
	for (UQuest* PlacedQuest : PlacedQuests)
	{
		PlacedQuest->ReconstructQuest();
	}
	// Available quests as well, cause may need to. Not sure.
	for (UQuest* AvailableQuest : AvailableQuests)
	{
		AvailableQuest->ReconstructQuest();
	}
}

void AFAGameMode::AddTakenQuest(UQuest* TakenQuest)
{
	TakenQuests.AddUnique(TakenQuest);
	PlacedQuests.RemoveSwap(TakenQuest);
	TakenQuest->QuestFinished.AddUObject(this, &AFAGameMode::RemoveFinishedQuest);
	TakenQuestsChanged.Broadcast();
}

void AFAGameMode::TakeQuest(TSubclassOf<UQuest> TakenQuest)
{
	UQuest* Quest = GetQuestFromClass(TakenQuest);
	if (Quest)
	{
		Quest->TakeQuest();
		AddTakenQuest(Quest);
	}
}

void AFAGameMode::RemoveFinishedQuest(UQuest* FinishedQuest)
{
	FinishedQuests.Add(FinishedQuest);
	TakenQuests.Remove(FinishedQuest);
	FinishedQuest->QuestFinished.Clear();
	TakenQuestsChanged.Broadcast();
}

void AFAGameMode::InitQuestsFromSaveFile(
	const TArray<UQuest*>& TakenQuestsLoaded,
	const TArray<UQuest*>& PlacedQuestsLoaded,
	const TArray<UQuest*>& FinishedQuestsLoaded)
{
	TakenQuests = TakenQuestsLoaded;
	PlacedQuests = PlacedQuestsLoaded;
	FinishedQuests = FinishedQuestsLoaded;
}

void AFAGameMode::ExpandMapNode(UMapNode* NodeToExpand)
{
	// Removing all inaccessible quests, which Player can't reach
	// All those quests get pushed back to AvailableQuests
	RemoveInaccessibleQuests(NodeToExpand);
	
	// Sampled quests from AvailableQuests for map generation
	TArray<UQuest*> QuestsToPlace = SampleQuestList(MaxNodeChildren);
	
	// Expanding selected node based on sample quests
	WorldMap->ExpandNode(NodeToExpand, QuestsToPlace);
	
	// Update placed quests, cause not all of them could be randomly placed
	// So they need to be added back to AvailableQuests
	UpdatePlacedQuests(NodeToExpand, QuestsToPlace);
}

void AFAGameMode::TravelPlayerToNode(UMapNode* NodeTravelTo)
{
	if (!NodeTravelTo)
	{
		UE_LOG(LogTemp, Error, TEXT("NodeTravelTo was null."));
		return;
	}

	// CHANGE LOCATION
	ChangeLocation(NodeTravelTo);
	
	UE_LOG(LogTemp, Display,
		TEXT("Player succesfully traveled to %s node."),
		*(NodeTravelTo->GetLocationInfo()->LocationName.ToString()));
	
	// EXPAND NODE
	ExpandMapNode(NodeTravelTo);
	WorldMap->SetCurrentNode(NodeTravelTo);
}

void AFAGameMode::ChangeLocation(const UMapNode* TargetLocation)
{
	// EMPTY LOCATION
	EmptyLocation();
	
	// SPAWN NEW CHARACTERS
	SpawnNewCharacters(TargetLocation);

	// GENERATE CARGO
	CargoDeliveryManager->GenerateCargoes(TargetLocation);
}

void AFAGameMode::LetPassengerInPlane(AFABasePassenger* PassengerToLetIn)
{
	if (SpacePlane->AssignPassenger(PassengerToLetIn))
	{
		const FVector MoveToSeatLocation =
			PassengerToLetIn->GetAssignedPassengerSeat()->GetMoveToLocation().GetLocation();
		UActionExecutorComponent* ActionExecutor = PassengerToLetIn->GetActionExecutorComponent();
		ActionExecutor->SetActions(
		{
			UCharacterMoveAction::CreateCharacterMoveAction(MoveToSeatLocation, ActionExecutor),
			UCharacterSitAction::CreateCharacterSitAction(PassengerToLetIn->GetAssignedPassengerSeat(), ActionExecutor)
		});
		ActionExecutor->ExecuteActions();
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("LetPassengerInPlane was called, but there were no free seats."));
	}
}

UQuest* AFAGameMode::GetQuestFromClass(TSubclassOf<UQuest> QuestClass) const
{
	for (UQuest* Quest : TakenQuests)
	{
		if (Quest->IsA(QuestClass.Get()))
		{
			return Quest;
		}
	}
	for (UQuest* Quest : PlacedQuests)
	{
		if (Quest->IsA(QuestClass.Get()))
		{
			return Quest;
		}
	}
	for (UQuest* Quest : AvailableQuests)
	{
		if (Quest->IsA(QuestClass.Get()))
		{
			return Quest;
		}
	}
	
	UE_LOG(LogTemp, Error,
		TEXT("%s quest wasn't recognise in FAGameMode's Quest Lists."),
		*(QuestClass.Get()->GetFullName()));
	return nullptr;
}

void AFAGameMode::ShowCargoPickMenu() const
{
	UUserWidget* CreatedWidget = CreateWidget(GetWorld(), CargoPickMenuWidgetClass);
	if (CreatedWidget)
	{
		// cannot assign to readonly type wtf?
		// CargoPickMenuWidget = CreatedWidget;
		CreatedWidget->AddToViewport(1);
	}
}

TArray<UQuest*> AFAGameMode::SampleQuestList(const int32 Num)
{
	TArray<UQuest*> QuestsToPlace;
	const int32 AvailableQuestsNum = FMath::Clamp(AvailableQuests.Num(), 0, Num);
	for (int i = 0; i < AvailableQuestsNum; ++i)
	{
		QuestsToPlace.Add(AvailableQuests.Pop(false));
	}

	return QuestsToPlace;
}

void AFAGameMode::UpdatePlacedQuests(const UMapNode* ExpandedNode, TArray<UQuest*>& QuestsToPlace)
{
	for (int i = 0; i < QuestsToPlace.Num(); ++i)
	{
		if (!QuestsToPlace[i]->GetIsPlaced())
		{
			AvailableQuests.Add(QuestsToPlace[i]);
			QuestsToPlace.RemoveAtSwap(i);
		}
	}
	UE_LOG(LogTemp, Display,
		TEXT("%d quests were placed during expand of %s node."),
		QuestsToPlace.Num(), *(ExpandedNode->GetName()));
	PlacedQuests.Append(QuestsToPlace);
}

void AFAGameMode::RemoveInaccessibleQuests(const UMapNode* NodeOfParentToIgnoreFrom)
{
	TArray<UQuest*> InaccessibleQuests;
	const UMapNode* CommonParent = NodeOfParentToIgnoreFrom->GetParentNode();
	for (int32 i = PlacedQuests.Num() - 1; i >= 0; --i)
	{
		if (PlacedQuests[i]->GetLocationsToGenerate().Num() == 0)
		{
			UE_LOG(LogTemp, Error,
				TEXT("No locations were linked to %s quest."),
				*(PlacedQuests[i]->GetQuestName()));
			continue;
		}
		const UMapNode* PlacedNodeParent =
			PlacedQuests[i]->GetLocationsToGenerate()[0]->CorrespondingNode->GetParentNode();
		if (PlacedNodeParent != CommonParent
			&& !PlacedQuests[i]->GetIsFamiliar())
		{
			InaccessibleQuests.Add(PlacedQuests[i]);
		}
	}
	for (UQuest* Quest : InaccessibleQuests)
	{
		Quest->SetIsPlaced(false);
		AvailableQuests.Add(Quest);
		PlacedQuests.RemoveSwap(Quest);
	}
}

void AFAGameMode::FillPassengers()
{
	const int32 ChildNodesNum =
			WorldMap->GetCurrentNode()->GetChildNodes().Num();
	if (ChildNodesNum == 0)
		return;
	
	// Initialized in constructor
	check(PassengersManager);
	const int32 PassengersNum = FMath::Clamp(
		FMath::RandRange(MinPassengers, MaxPassengers),
		MinPassengers,
		DebugPassengersSpawnLocations.Num());
	for (int i = 0; i < PassengersNum; ++i)
	{
		ULocationInfo* Destination =
			WorldMap->GetCurrentNode()->GetChildNodes()[FMath::RandHelper(ChildNodesNum)]->GetLocationInfo();
		PassengersManager->CreatePassenger(
			DebugPassengersSpawnLocations[i]->GetActorTransform(),
			Destination)
				->SpawnDefaultController();
	}
}

void AFAGameMode::EmptyLocation()
{
	for (AFABaseCharacter* SpawnedCharacter : SpawnedCharacters)
	{
		SpawnedCharacter->Destroy();
	}
	SpawnedCharacters.Empty();
	PassengersManager->ClearRedundantPassengers();
}

void AFAGameMode::SpawnNewCharacters(const UMapNode* NodeTravelTo)
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	const auto& CharactersToSpawn =
		NodeTravelTo->GetLocationInfo()->ResidentsCharacters;
	const int32 Num = FMath::Min(CharactersToSpawn.Num(), DebugCharactersSpawnLocations.Num());
	for (int32 i = 0; i < Num; ++i)
	{
		AActor* SpawnedCharacter =
			World->SpawnActor(
				CharactersToSpawn[i].Get(),
				&DebugCharactersSpawnLocations[i]->GetActorTransform());
		if (SpawnedCharacter)
			SpawnedCharacters.Add(Cast<AFABaseCharacter>(SpawnedCharacter));
	}
	FillPassengers();
}
