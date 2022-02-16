// Fill out your copyright notice in the Description page of Project Settings.


#include "FAGameMode.h"
#include "Engine/AssetManager.h"
#include "Quests/Quest.h"
#include "Characters/FABaseCharacter.h"

void AFAGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	DebugCharactersSpawnLocations.Empty();

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

	// Not sure where to call
	LocationLoadedHandle();

	TArray<UQuest*> QuestsToPlace = SampleQuestList(MaxNodeChildren);

	WorldMap = NewObject<UMapGraph>(this);
	if (WorldMap)
	{
		WorldMap->GenerateMap(WorldMapMaxDepth, QuestsToPlace);
	}
	UpdatePlacedQuests(WorldMap->GetRootNode(), QuestsToPlace);
}

void AFAGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AFAGameMode::LocationLoadedHandle()
{
	// TODO: In fact I also need to init their structure at the moment they got taken by Player.
	// Updating all quests structure.
	for (UQuest* TakenQuest : TakenQuests)
	{
		TakenQuest->Init();
	}
	// Available quests as well, cause may need to. Not sure.
	for (UQuest* AvailableQuest : AvailableQuests)
	{
		AvailableQuest->Init();
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
	for (UQuest* Quest : PlacedQuests)
	{
		if (Quest->StaticClass() == TakenQuest.Get())
		{
			Quest->TakeQuest();
			AddTakenQuest(Quest);
			break;
		}
	}

	UE_LOG(LogTemp, Error,
		TEXT("%s quest wasn't recognise in PlacedQuests and was not taken."),
		*(TakenQuest.Get()->GetFullName()));
}

void AFAGameMode::RemoveFinishedQuest(UQuest* FinishedQuest)
{
	FinishedQuests.Add(FinishedQuest);
	TakenQuests.Remove(FinishedQuest);
	FinishedQuest->QuestFinished.Clear();
	TakenQuestsChanged.Broadcast();
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
	// EMPTY LOCATION
	{
		for (AFABaseCharacter* SpawnedCharacter : SpawnedCharacters)
		{
			SpawnedCharacter->Destroy();
		}
		SpawnedCharacters.Empty();
	}

	// SPAWN NEW CHARACTERS
	{
		UWorld* World = GetWorld();
		if (!World)
			return;
		const auto& CharactersToSpawn =
			NodeTravelTo->GetLocationInfo()->ResidentsCharacters;
		for (int i = 0; i < CharactersToSpawn.Num(); ++i)
		{
			AActor* SpawnedCharacter =
				World->SpawnActor(
					CharactersToSpawn[i].Get(),
					&DebugCharactersSpawnLocations[i]->GetActorTransform());
			if (SpawnedCharacter)
				SpawnedCharacters.Add(Cast<AFABaseCharacter>(SpawnedCharacter));
		}
	}
	UE_LOG(LogTemp, Display,
		TEXT("Player succesfully traveled to %s node."),
		*(NodeTravelTo->GetLocationInfo()->LocationName.ToString()));
	
	// EXPAND NODE
	ExpandMapNode(NodeTravelTo);
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
	const UMapNode* CommonParent = NodeOfParentToIgnoreFrom->GetParentNode();
	for (int i = PlacedQuests.Num() - 1; i >= 0; --i)
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
			PlacedQuests[i]->SetIsPlaced(false);
			AvailableQuests.Add(PlacedQuests[i]);
			PlacedQuests.RemoveAtSwap(i);
		}
	}
}
