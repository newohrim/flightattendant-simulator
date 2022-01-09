// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestNode.generated.h"

class UQuestTransition;
class UQuestNode;

DECLARE_DELEGATE_OneParam(FNodeCompleted, UQuestNode*)

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UQuestNode : public UObject
{
	GENERATED_BODY()

public:
	FNodeCompleted NodeCompleted;

	// Obsolete test function
	void Init();
	void CompleteNode(UQuestNode* NextNode);
	UFUNCTION(BlueprintCallable)
	void AddTransition(UQuestTransition* Transition);
	bool IsLast() const { return ChildTransitions.Num() == 0; }
	UFUNCTION(BlueprintCallable)
	UQuestTransition* CreateTransition();

protected:
	TArray<UQuestTransition*> ChildTransitions;
};

