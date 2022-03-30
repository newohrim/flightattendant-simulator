// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quests/QuestTransition.h"
#include "TravelEndTransition.generated.h"

class UMapNode;

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UTravelEndTransition : public UQuestTransition
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UTravelEndTransition* CreateTravelEndTransition(UMapNode* Destination, UObject* Outer);

protected:
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UMapNode> DestinationNode;

	UFUNCTION()
	void PlayerArrivedHandle(UMapNode* Destination);
};
