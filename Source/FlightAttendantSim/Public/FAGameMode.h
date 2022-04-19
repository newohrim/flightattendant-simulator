// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldMap/MapNode.h"
#include "WorldMap/MapGraph.h"
#include "GameFramework/GameModeBase.h"
#include "FAGameMode.generated.h"

#define FAGAMEMODE Cast<AFAGameMode>(GetWorld()->GetAuthGameMode())

class AFABaseCharacter;
class AFABasePassenger;
class UMapNode;
class UQuest;
class USpacePlaneComponent;
class UPassengersManagerComponent;
class UCargoManagerComponent;
class UPDAMessengerComponent;
class UFlightControlComponent;
class UGameEconomyComponent;
class USaveGameComponent;
class UWaypointsComponent;
class ILocationGenerator;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakenQuestsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLocationLoaded, const UMapNode*, LoadedLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestsCompleted, const UQuest*, UQuest);

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API AFAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FLocationLoaded LocationLoaded;
	UPROPERTY(BlueprintAssignable)
	FQuestsCompleted QuestCompleted;
	
	AFAGameMode();
	
	UPROPERTY(BlueprintAssignable)
	FTakenQuestsChanged TakenQuestsChanged;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Logout(AController* Exiting) override;
	virtual void PostLoadInitialization();
	void LocationLoadedHandle();

	UFUNCTION(BlueprintCallable)
	void TakeQuest(TSubclassOf<UQuest> TakenQuest);

	const TArray<UQuest*>& GetAvailableQuests() const { return AvailableQuests; }
	const TArray<UQuest*>& GetTakenQuests() const { return TakenQuests; }
	const TArray<UQuest*>& GetPlacedQuests() const { return PlacedQuests; }
	const TArray<UQuest*>& GetFinishedQuests() const { return FinishedQuests; }
	void InitQuestsFromSaveFile(
		const TArray<UQuest*>& AvailableQuestsLoaded,
		const TArray<UQuest*>& TakenQuestsLoaded,
		const TArray<UQuest*>& PlacedQuestsLoaded, const TArray<UQuest*>& FinishedQuestsLoaded);

	UFUNCTION(BlueprintCallable)
	void ExpandMapNode(UMapNode* NodeToExpand);

	UFUNCTION(BlueprintCallable)
	void TravelPlayerToNode(UMapNode* NodeTravelTo);

	UFUNCTION(BlueprintCallable)
	void LetPassengerInPlane(AFABasePassenger* PassengerToLetIn);

	UFUNCTION(BlueprintCallable)
	UQuest* GetQuestFromClass(TSubclassOf<UQuest> QuestClass) const;

	UFUNCTION(BlueprintCallable)
	UPassengersManagerComponent* GetPassengerManager() const { return PassengersManager; }

	UFUNCTION(BlueprintCallable)
	UCargoManagerComponent* GetCargoManager() const { return CargoDeliveryManager; }

	UFUNCTION(BlueprintCallable)
	UPDAMessengerComponent* GetPDAMessenger() const { return PDAMessenger; }

	UFlightControlComponent* GetFlightController() const { return FlightController; }

	UGameEconomyComponent* GetEconomyComponent() const { return EconomyComponent; }

	USaveGameComponent* GetSaveGameComponent() const { return SaveGameComponent; }

	UWaypointsComponent* GetWaypointsComponent() const { return WaypointsComponent; }

	UMapGraph* GetWorldMap() const { return WorldMap; }

	USpacePlaneComponent* GetSpacePlane() const { return SpacePlane; }

	void ShowCargoPickMenu() const;

	void SetPlayerInitialTransform(const FTransform& InitTransform) { InitialPlayerTransform = InitTransform; }
	
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
	TSubclassOf<UFlightControlComponent> DefaultFlightControllerClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> DefaultLocationGeneratorClass;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UFlightControlComponent* FlightController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGameEconomyComponent* EconomyComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USaveGameComponent* SaveGameComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UWaypointsComponent* WaypointsComponent;

	// Global map graph of the game world.
	UPROPERTY(BlueprintReadOnly)
	UMapGraph* WorldMap = nullptr;
	// Player's spaceship abstraction
	UPROPERTY(BlueprintReadOnly)
	USpacePlaneComponent* SpacePlane = nullptr;
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<ILocationGenerator> LocationGenerator;
	
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

	FTransform InitialPlayerTransform;

	TArray<UQuest*> SampleQuestList(const int32 Num);
	void UpdatePlacedQuests(const UMapNode* ExpandedNode, TArray<UQuest*>& QuestsToPlace);
	void RemoveInaccessibleQuests(const UMapNode* NodeOfParentToIgnoreFrom);
	void FillPassengers();
	
	void FillPassengersPostLoad() const;

	void EmptyLocation();

	void SpawnNewCharacters(const UMapNode* NodeTravelTo, bool IsInitial);

	void ChangeLocation(const UMapNode* TargetLocation, const bool IsInitial = false);

	UFUNCTION(BlueprintNativeEvent)
	void SpaceShipBrokenHandle();

private:
	UPROPERTY()
	TArray<AFABaseCharacter*> SpawnedCharacters;

	bool LoadSucceeded = true;

	void AddTakenQuest(UQuest* TakenQuest);
	void RemoveFinishedQuest(UQuest* FinishedQuest);
};
