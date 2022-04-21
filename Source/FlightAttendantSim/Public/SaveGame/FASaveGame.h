// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Passengers/PassengerSpawnParams.h"
#include "Components/ItemsInventoryComponent.h"
#include "CargoDelivery/CargoInfo.h"
#include "GameFramework/SaveGame.h"
#include "FASaveGame.generated.h"

class ULocationInfo;
class UMapGraph;
class UMapNode;
class UQuest;
class USpacePlaneComponent;
class UPassengersManagerComponent;

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

	UPROPERTY()
	TArray<int32> PlacedNodeIndices;
};

USTRUCT()
struct FCargoData
{
	GENERATED_BODY()

	UPROPERTY()
	FCargoInfo CargoInfo;

	UPROPERTY()
	int32 LocationFromIndex = -1;

	UPROPERTY()
	int32 LocationToIndex = -1;
};

USTRUCT()
struct FPassengerData
{
	GENERATED_BODY()
	
	UPROPERTY()
	FPassengerSpawnParams SpawnParams;

	UPROPERTY()
	int32 LocationHeadingToIndex = -1;
};

USTRUCT()
struct FSpacePlaneData
{
	GENERATED_BODY()

	TSoftObjectPtr<USpacePlaneComponent> SpacePlane = nullptr;

	UPROPERTY()
	TArray<uint8> SpacePlaneData;

	UPROPERTY()
	TArray<uint8> HealthComponentData;

	UPROPERTY()
	TArray<uint8> CargoCellData;

	UPROPERTY()
	TArray<FPassengerData> AssignedPassengers;
};

USTRUCT()
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = PlayerStats)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = PlayerStats)
	int32 PlayerMoney = 0;

	UPROPERTY(VisibleAnywhere, Category = PlayerStats)
	FTransform PlayerTransform;
};

USTRUCT()
struct FInventoryItemData
{
	GENERATED_BODY()

	UPROPERTY()
	TEnumAsByte<EInventoryItemType> ItemType;

	UPROPERTY()
	int32 Count = 0;
};

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UFASaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = PlayerData)
	FPlayerData PlayerData;

	UPROPERTY(VisibleAnywhere, Category = WorldMap)
	TArray<FMapNodeData> WorldMap;
	
	UPROPERTY(VisibleAnywhere, Category = Quests)
	TArray<FQuestData> AvailableQuests;

	UPROPERTY(VisibleAnywhere, Category = Quests)
	TArray<FQuestData> TakenQuests;

	UPROPERTY(VisibleAnywhere, Category = Quests)
	TArray<FQuestData> PlacedQuests;

	UPROPERTY(VisibleAnywhere, Category = Quests)
	TArray<FQuestData> FinishedQuests;

	UPROPERTY(VisibleAnywhere, Category = Cargoes)
	TArray<FCargoData> TakenCargoes;

	UPROPERTY(VisibleAnywhere, Category = Cargoes)
	TArray<FCargoData> AvailableCargoes;

	UPROPERTY(VisibleAnywhere, Category = Inventory)
	TArray<FInventoryItemData> PlayerInventory;

	UPROPERTY(VisibleAnywhere, Category = SpacePlane)
	FSpacePlaneData SpacePlaneData;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UFASaveGame();

	void PostLoadInitialization();
	
	void SaveWorldMap(const UMapGraph* MapGraph);

	void SaveSpacePlane(USpacePlaneComponent* SpacePlaneComponent);

	void SavePassengers(const UPassengersManagerComponent* PassengersManager);

	int32 FindNode(const UMapNode* Node) const;

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
