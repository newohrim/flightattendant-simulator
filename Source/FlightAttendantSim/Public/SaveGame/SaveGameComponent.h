// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FASaveGame.h"
#include "WorldMap/MapNode.h"
#include "Components/ActorComponent.h"
#include "SaveGameComponent.generated.h"

struct FMapNodeData;
struct FQuestData;
class UFASaveGame;
class UMapNode;
class UCargoManagerComponent;
class UCargoCellComponent;
enum EQuestStatus;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API USaveGameComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USaveGameComponent();

	UFUNCTION(BlueprintCallable)
	bool SaveGame();

	UFUNCTION(BlueprintCallable)
	bool LoadGame();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SaveGameProperties")
	FString MainSaveGameSlotName = "MainSaveSlot";

	UPROPERTY(EditDefaultsOnly, Category = "SaveGameProperties")
	uint32 MainSaveGameSlotIndex = 0;
	
	virtual void PopulateSaveFile(UFASaveGame* SaveGame);

	virtual void GatherLoadedSaveFile(UFASaveGame* SaveGame);

private:
	static void PopulateQuests(TArray<FQuestData>& ToList, const TArray<UQuest*>& FromList, UFASaveGame* SaveGame);
	
	static TArray<UQuest*> GatherQuests(
		const TArray<FQuestData>& FromList,
		const EQuestStatus QuestStatus,
		UFASaveGame* SaveGame, UObject* Outer);

	static void PopulateCargoes(
		TArray<FCargoData>& ToList,
		const TArray<FCargoInfo>& FromList,
		const UFASaveGame* SaveGame);

	template<typename F>
	static void GatherCargoes(
		TArray<FCargoData>& FromList,
		const UFASaveGame* SaveGame,
		F&& AddToListFunc);

	static TArray<FPassengerSpawnParams> GatherPassengers(
		TArray<FPassengerData>& FromList,
		const UFASaveGame* SaveGame);

	static void PopulateInventory(
		const UItemsInventoryComponent* ItemsInventory,
		UFASaveGame* SaveGame);

	static void GatherInventory(
		UItemsInventoryComponent* ItemsInventory,
		const UFASaveGame* SaveGame);

public:
	UMapNode* ReconstructWorldMap(
		const FMapNodeData& NodeData,
		const UFASaveGame* SaveGame,
		UObject* Outer = nullptr);
};

UCLASS()
class FLIGHTATTENDANTSIM_API UMapNodeReconstructed : public UMapNode
{
	GENERATED_BODY()
private:
	friend UMapNode* USaveGameComponent::ReconstructWorldMap(
		const FMapNodeData& NodeData,
		const UFASaveGame* SaveGame,
		UObject* Outer);
};
