// Fill out your copyright notice in the Description page of Project Settings.


#include "FAGameMode.h"
#include "Engine/AssetManager.h"
#include "Quests/Quest.h"
#include "WorldMap/MapGraph.h"

void AFAGameMode::BeginPlay()
{
	Super::BeginPlay();

	UAssetManager& AssetManager = UAssetManager::Get();
	FPrimaryAssetType QuestAssetType = TEXT("Quest");
	TArray<FPrimaryAssetId> QuestAssetsIds; 
	AssetManager.GetPrimaryAssetIdList(QuestAssetType, QuestAssetsIds);
	for (const FPrimaryAssetId& QuestId : QuestAssetsIds)
	{
		FAssetData AssetData;
		AssetManager.GetPrimaryAssetData(QuestId, AssetData);
		auto Temp = Cast<UBlueprint>(AssetData.GetAsset());
		UQuest* LoadedQuest = NewObject<UQuest>(this, Temp->GeneratedClass);
		// Adding all quests just for debug
		if (LoadedQuest)
			AvailableQuests.Add(LoadedQuest);
	}
	
	LocationLoadedHandle();

	// Just for debug purpose
	for (UQuest* Quest : AvailableQuests)
	{
		Quest->TakeQuest();
		AvailableQuests.Remove(Quest);
	}
	
	// Just for debug purpose
	WorldMap = NewObject<UMapGraph>(this);
	if (WorldMap)
	{
		WorldMap->GenerateMap(WorldMapMaxDepth);
	}
}

void AFAGameMode::LocationLoadedHandle()
{
	// Updating all quests structure.
	for (UQuest* TakenQuest : TakenQuests)
	{
		TakenQuest->Init();
	}
	// Available quests as well, cause may need to.
	for (UQuest* AvailableQuest : AvailableQuests)
	{
		AvailableQuest->Init();
	}
}

void AFAGameMode::AddTakenQuest(UQuest* TakenQuest)
{
	TakenQuests.AddUnique(TakenQuest);
	TakenQuestsChanged.Broadcast();
}

void AFAGameMode::RemoveFinishedQuest(UQuest* FinishedQuest)
{
	TakenQuests.Remove(FinishedQuest);
	TakenQuestsChanged.Broadcast();
}
