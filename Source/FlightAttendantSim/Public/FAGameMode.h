// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldMap/MapNode.h"
#include "WorldMap/MapGraph.h"
#include "GameFramework/GameModeBase.h"
#include "FAGameMode.generated.h"

class AFABaseCharacter;
class AFABasePassenger;
class UMapNode;
class UQuest;
class USpacePlaneComponent;
class UPassengersManagerComponent;
class UCargoManagerComponent;
class UPDAMessengerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakenQuestsChanged);

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API AFAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFAGameMode();
	
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

	UFUNCTION(BlueprintCallable)
	void LetPassengerInPlane(AFABasePassenger* PassengerToLetIn);

	UFUNCTION(BlueprintCallable)
	UPassengersManagerComponent* GetPassengerManager() const { return PassengersManager; }

	UFUNCTION(BlueprintCallable)
	UCargoManagerComponent* GetCargoManager() const { return CargoDeliveryManager; }

	UFUNCTION(BlueprintCallable)
	UPDAMessengerComponent* GetPDAMessenger() const { return PDAMessenger; }

	USpacePlaneComponent* GetSpacePlane() const { return SpacePlane; }

	void ShowCargoPickMenu() const;
	
protected:
	// The depth of final node
	constexpr static int32 WorldMapMaxDepth = 10;
	UPROPERTY(EditDefaultsOnly, Category = "MapGeneration")
	int32 MaxNodeChildren = 3;
	UPROPERTY(EditDefaultsOnly, Category = "MapGeneration")
	int32 MinPassengers = 3;
	UPROPERTY(EditDefaultsOnly, Category = "MapGeneration")
	int32 MaxPassengers = 6;
	UPROPERTY(BlueprintReadWrite, Category = "MapGeneration")
	TArray<AActor*> DebugCharactersSpawnLocations;
	UPROPERTY(BlueprintReadWrite, Category = "MapGeneration")
	TArray<AActor*> DebugPassengersSpawnLocations;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CargoPickMenuWidgetClass;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CargoPickMenuWidget = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UPassengersManagerComponent* PassengersManager;
	UPROPERTY(EditDefaultsOnly)
	UCargoManagerComponent* CargoDeliveryManager;
	UPROPERTY(EditDefaultsOnly)
	UPDAMessengerComponent* PDAMessenger;

	// Global map graph of the game world.
	UPROPERTY(BlueprintReadOnly)
	UMapGraph* WorldMap = nullptr;
	// Player's spaceship abstraction
	UPROPERTY(BlueprintReadOnly)
	USpacePlaneComponent* SpacePlane = nullptr;
	
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
	void FillPassengers();

private:
	UPROPERTY()
	TArray<AFABaseCharacter*> SpawnedCharacters;

	void AddTakenQuest(UQuest* TakenQuest);
	void RemoveFinishedQuest(UQuest* FinishedQuest);
};
