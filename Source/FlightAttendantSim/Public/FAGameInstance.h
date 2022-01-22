// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FAGameInstance.generated.h"

class UMapGraph;
class UQuest;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakenQuestsChanged);

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UFAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FTakenQuestsChanged TakenQuestsChanged;
	
	virtual void Init() override;
	
	void AddTakenQuest(UQuest* TakenQuest)
	{
		TakenQuests.AddUnique(TakenQuest);
		TakenQuestsChanged.Broadcast();
	}
	void RemoveFinishedQuest(UQuest* FinishedQuest)
	{
		TakenQuests.Remove(FinishedQuest);
		TakenQuestsChanged.Broadcast();
	}
	
protected:
	// The depth of final node
	constexpr static int32 WorldMapMaxDepth = 10;
	
	UPROPERTY(BlueprintReadOnly)
	UMapGraph* WorldMap;
	UPROPERTY(BlueprintReadOnly)
	TArray<UQuest*> TakenQuests;
};
