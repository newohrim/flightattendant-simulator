// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapNode.h"
#include "GameFramework/GameModeBase.h"
#include "WorldMap/MapGraph.h"
#include "FAGameMode.generated.h"

class AFABaseCharacter;
class UMapNode;
class UQuest;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakenQuestsChanged);

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API AFAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FTakenQuestsChanged TakenQuestsChanged;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	void LocationLoadedHandle();
	
	void TakeQuest(TSubclassOf<UQuest> TakenQuest);

	UFUNCTION(BlueprintCallable)
	void ExpandMapNode(UMapNode* NodeToExpand);

	UFUNCTION(BlueprintCallable)
	void TravelPlayerToNode(UMapNode* NodeTravelTo);
	
protected:
	// The depth of final node
	constexpr static int32 WorldMapMaxDepth = 10;
	UPROPERTY(EditDefaultsOnly, Category = "MapGeneration")
	int32 MaxNodeChildren = 3;
	UPROPERTY(BlueprintReadWrite, Category = "MapGeneration")
	TArray<AActor*> DebugCharactersSpawnLocations;

	// Global map graph of the game world.
	UPROPERTY(BlueprintReadOnly)
	UMapGraph* WorldMap;
	// Quests taken by player.
	UPROPERTY(BlueprintReadOnly)
	TArray<UQuest*> TakenQuests;
	// All loaded quests which can be used to gen map
	UPROPERTY(BlueprintReadOnly)
	TArray<UQuest*> AvailableQuests;
	// Quests which are placed on map
	UPROPERTY(BlueprintReadOnly)
	TArray<UQuest*> PlacedQuests;
	// Finished by Player quests
	UPROPERTY(BlueprintReadOnly)
	TArray<UQuest*> FinishedQuests;

	TArray<UQuest*> SampleQuestList(const int32 Num);
	void UpdatePlacedQuests(const UMapNode* ExpandedNode, TArray<UQuest*>& QuestsToPlace);
	void RemoveInaccessibleQuests(const UMapNode* NodeOfParentToIgnoreFrom);

private:
	UPROPERTY()
	TArray<AFABaseCharacter*> SpawnedCharacters;

	void AddTakenQuest(UQuest* TakenQuest);
	void RemoveFinishedQuest(UQuest* FinishedQuest);
};
