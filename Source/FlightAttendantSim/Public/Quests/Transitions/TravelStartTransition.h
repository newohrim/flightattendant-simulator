// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quests/QuestTransition.h"
#include "TravelStartTransition.generated.h"

class UMapNode;

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UTravelStartTransition : public UQuestTransition
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UTravelStartTransition* CreateTravelStartTransition(UMapNode* Destination, UObject* Outer);

protected:
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UMapNode> DestinationNode;

	UFUNCTION()
	void PlayerStartTravelHandle(UMapNode* Destination);
};
