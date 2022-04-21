// Fill out your copyright notice in the Description page of Project Settings.


#include "FAGameMode.h"

#include "EngineUtils.h"
#include "FAGameInstance.h"
#include "Components/LocationGenerator.h"
#include "SpacePlane/SpacePlaneHealthComponent.h"
#include "Flight/FlightControlComponent.h"
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
#include "Components/WaypointsComponent.h"
#include "Components/ItemsInventoryComponent.h"

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
	WaypointsComponent =
		CreateDefaultSubobject<UWaypointsComponent>("WaypointsComponent");
	ItemsInventory =
		CreateDefaultSubobject<UItemsInventoryComponent>("ItemsInventoryComponent");

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

	const UFAGameInstance* GameInstance = Cast<UFAGameInstance>(GetWorld()->GetGameInstance());
	
	// Load game
	if (GameInstance->IsNewGame || !SaveGameComponent->LoadGame())
	{
		LoadSucceeded = false;
		
		{ // TODO: Decompose to protected func
			const UAssetManager& AssetManager = UAssetManager::Get();
			
			const FPrimaryAssetType QuestAssetType = TEXT("Quest");
			TArray<FPrimaryAssetId> QuestAssetsIds;
			FStreamableManager AssetStream;
			AssetManager.GetPrimaryAssetIdList(QuestAssetType, QuestAssetsIds);
			for (int32 i = 0; i < QuestAssetsIds.Num(); ++i)
			{
				FAssetData AssetData;
				AssetManager.GetPrimaryAssetData(QuestAssetsIds[i], AssetData);
				FSoftObjectPath QuestClassPtr = AssetData.ToSoftObjectPath();
				QuestClassPtr.SetPath(QuestClassPtr.GetAssetPathString().Append("_C"));
				UObject* QuestClass = AssetStream.LoadSynchronous(QuestClassPtr);
				/*
				TArray<UObject*> AssetObjects;
				FindOrLoadAssetsByPath(AssetData.PackagePath.ToString(), AssetObjects, EngineUtils::ATL_Class);
				UBlueprintGeneratedClass* QuestClass = nullptr;
				const FString AssetName = AssetData.AssetName.ToString().Append("_C");
				for (int32 j = 0; j < AssetObjects.Num(); ++j)
				{
					if (AssetObjects[j]->GetName().Compare(AssetName) == 0)
					{
						QuestClass = Cast<UBlueprintGeneratedClass>(AssetObjects[j]);
						break;
					}
				}
				*/
				if (QuestClass)
				{
					UQuest* LoadedQuest =
						NewObject<UQuest>(this, Cast<UBlueprintGeneratedClass>(QuestClass));
					if (LoadedQuest)
					{
						AvailableQuests.Add(LoadedQuest);
					}
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

	{
		USpacePlaneHealthComponent* HealthComponent = SpacePlane->GetSpacePlaneHealth();
		check(HealthComponent);
		HealthComponent->SpacePlaneDie.AddDynamic(this, &AFAGameMode::SpaceShipBrokenHandle);
	}
	
	PostLoadInitialization();
}

void AFAGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	//SaveGameComponent->SaveGame();
}

void AFAGameMode::PostLoadInitialization()
{
	LocationGenerator = GetWorld()
		->SpawnActor(DefaultLocationGeneratorClass, &FTransform::Identity);
	ChangeLocation(WorldMap->GetCurrentNode(), true);
	if (LoadSucceeded)
	{
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)
			->SetActorTransform(InitialPlayerTransform);
	}
}

void AFAGameMode::LocationLoadedHandle()
{
	// TODO: In fact I also need to init their structure at the moment they got taken by Player.
	// Updating all quests structure.
	for (UQuest* PlacedQuest : PlacedQuests)
	{
		PlacedQuest->ReconstructQuest();
	}
	// Available quests as well, cause may need too. Not sure.
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
	if (Quest && Quest->QuestStatus == EQuestStatus::Waiting)
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
	QuestCompleted.Broadcast(FinishedQuest);
}

void AFAGameMode::InitQuestsFromSaveFile(
	const TArray<UQuest*>& AvailableQuestsLoaded,
	const TArray<UQuest*>& TakenQuestsLoaded,
	const TArray<UQuest*>& PlacedQuestsLoaded,
	const TArray<UQuest*>& FinishedQuestsLoaded)
{
	AvailableQuests = AvailableQuestsLoaded;
	TakenQuests = TakenQuestsLoaded;
	PlacedQuests = PlacedQuestsLoaded;
	FinishedQuests = FinishedQuestsLoaded;
	for (UQuest* Quest :TakenQuests)
	{
		Quest->QuestFinished.AddUObject(this, &AFAGameMode::RemoveFinishedQuest);
	}
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

	// TAKE 1 HYPERCHARGE
	if (!ItemsInventory->RemoveItems(EInventoryItemType::Hypercharge))
		return;

	// EXPAND NODE
	ExpandMapNode(NodeTravelTo);
	WorldMap->SetCurrentNode(NodeTravelTo);

	// CHANGE LOCATION
	ChangeLocation(NodeTravelTo, false);

	UE_LOG(LogTemp, Display,
		TEXT("Player succesfully traveled to %s node."),
		*NodeTravelTo->GetLocationInfo()->LocationName.ToString());
}

void AFAGameMode::ChangeLocation(const UMapNode* TargetLocation, const bool IsInitial)
{
	// EMPTY LOCATION
	EmptyLocation();

	LocationGenerator->Execute_GenerateLocation(
			LocationGenerator.GetObject(),
			WorldMap->GetCurrentNode()->GetLocationInfo());
	
	// SPAWN NEW CHARACTERS
	SpawnNewCharacters(TargetLocation, IsInitial);

	// GENERATE CARGO
	if (!LoadSucceeded)
		CargoDeliveryManager->GenerateCargoes(TargetLocation);

	// LOCATION LOAD HANDLE?
	// ...
	LocationLoaded.Broadcast(TargetLocation);
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

TArray<AFABaseCharacter*> AFAGameMode::GetCharactersOfClass(const TSoftClassPtr<AFABaseCharacter>& CharacterClass) const
{
	TArray<AFABaseCharacter*> ResultArray;
	for (AFABaseCharacter* Character : SpawnedCharacters)
	{
		if (Character->IsA(CharacterClass.Get()))
		{
			ResultArray.Add(Character);
		}
	}
	return ResultArray;
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
	//const int32 ChildNodesNum =
	//		WorldMap->GetCurrentNode()->GetChildNodes().Num();
	//if (ChildNodesNum == 0)
	//	return;
	
	// Initialized in constructor
	check(PassengersManager);
	const int32 PassengersNum = FMath::Clamp(
		FMath::RandRange(MinPassengers, MaxPassengers),
		MinPassengers,
		DebugPassengersSpawnLocations.Num());
	const TArray<UMapNode*> AccessibleNodes =
		WorldMap->GetCurrentNode()->GetAccessibleNodes();
	for (int i = 0; i < PassengersNum; ++i)
	{
		ULocationInfo* Destination =
			AccessibleNodes[FMath::RandHelper(AccessibleNodes.Num())]->GetLocationInfo();
		PassengersManager->CreatePassenger(
			DebugPassengersSpawnLocations[i]->GetActorTransform(),
			Destination)
				->SpawnDefaultController();
	}
}

void AFAGameMode::FillPassengersPostLoad() const
{
	// Spawn buffered from save file passengers
	while (PassengersManager->PopBufferedPassenger());
}

void AFAGameMode::EmptyLocation()
{
	for (AFABaseCharacter* SpawnedCharacter : SpawnedCharacters)
	{
		SpawnedCharacter->Destroy();
	}
	SpawnedCharacters.Empty();
	PassengersManager->ClearRedundantPassengers();
	LocationGenerator->Execute_EmptyLocation(LocationGenerator.GetObject());
}

void AFAGameMode::SpawnNewCharacters(const UMapNode* NodeTravelTo, bool IsInitial)
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	const auto& CharactersToSpawn =
		NodeTravelTo->GetLocationInfo()->ResidentsCharacters;
	const int32 Num = FMath::Min(CharactersToSpawn.Num(), DebugCharactersSpawnLocations.Num());
	for (int32 i = 0; i < Num; ++i)
	{
		AFABaseCharacter* SpawnedCharacter =
			Cast<AFABaseCharacter>(World->SpawnActor(
				CharactersToSpawn[i].Get(),
				&DebugCharactersSpawnLocations[i]->GetActorTransform()));
		if (SpawnedCharacter)
		{
			SpawnedCharacter->SpawnDefaultController();
			SpawnedCharacters.Add(Cast<AFABaseCharacter>(SpawnedCharacter));
		}
	}
	UE_LOG(LogTemp, Display, TEXT("%d"), LoadSucceeded);
	if (!LoadSucceeded)
	{
		FillPassengers();
	}
	else if (!IsInitial)
	{
		FillPassengers();
	}
	else
	{
		FillPassengersPostLoad();
	}
}

void AFAGameMode::SpaceShipBrokenHandle_Implementation()
{
	// PLACEHOLDER
}
