// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FASaveGame.generated.h"

class ULocationInfo;
class UMapGraph;
class UMapNode;
class UQuest;

USTRUCT()
struct FMapNodeData
{
	GENERATED_BODY()

	TSoftObjectPtr<ULocationInfo> LocationInfo = nullptr;

	UPROPERTY()
	TArray<uint8> LocationData;

	UPROPERTY()
	TArray<int32> Children;

	UPROPERTY()
	bool IsCurrent = false;
};

USTRUCT()
struct FQuestData
{
	GENERATED_BODY()
	
	UPROPERTY()
	TSoftClassPtr<UQuest> QuestClass;

	UPROPERTY()
	int32 CurrentNodeIndex = -1;
};

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UFASaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = PlayerStats)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = PlayerStats)
	int32 PlayerMoney = 0;

	UPROPERTY(VisibleAnywhere, Category = WorldMap)
	TArray<FMapNodeData> WorldMap;

	UPROPERTY(VisibleAnywhere, Category = Quests)
	TArray<FQuestData> TakenQuests;

	UPROPERTY(VisibleAnywhere, Category = Quests)
	TArray<FQuestData> PlacedQuests;

	UPROPERTY(VisibleAnywhere, Category = Quests)
	TArray<FQuestData> FinishedQuests;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UFASaveGame();

	void PostLoadInitialization();
	
	void SaveWorldMap(const UMapGraph* MapGraph);

	FMapNodeData GetRootNode() const
	{
		if (WorldMap.Num() > 0)
			return WorldMap.Last();
		return FMapNodeData();
	}

private:
	FMapNodeData MapGraphTraversalWrite(const UMapNode* Node, const UMapNode* CurrentNode);

	void MapGraphTraversalRead();

	static void SerializeObject(UObject* Object, TArray<uint8>& OutputData);
	
	static void DeserializeObject(UObject* Object, TArray<uint8>& InputData);
};
