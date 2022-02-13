// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/Object.h"
#include "FAGameMode.generated.h"

class UMapGraph;
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
	
	virtual void BeginPlay() override;
	void LocationLoadedHandle();
	
	void AddTakenQuest(UQuest* TakenQuest);
	void RemoveFinishedQuest(UQuest* FinishedQuest);
	
protected:
	// The depth of final node
	constexpr static int32 WorldMapMaxDepth = 10;

	// Global map graph of the game world.
	UPROPERTY(BlueprintReadOnly)
	UMapGraph* WorldMap;
	// Quests taken by player.
	UPROPERTY(BlueprintReadOnly)
	TArray<UQuest*> TakenQuests;
	// Quests which are available to take on the current location.
	UPROPERTY(BlueprintReadOnly)
	TArray<UQuest*> AvailableQuests;
};
