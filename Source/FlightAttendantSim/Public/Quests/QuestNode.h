// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestNode.generated.h"

class UQuestTransition;

DECLARE_DELEGATE_OneParam(FNodeCompleted, UQuestTransition*)

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
	void CompleteNode(UQuestTransition* ExecutedTransition);
	UFUNCTION(BlueprintCallable)
	UQuestTransition* AddTransition(const FString& QuestDescription, UQuestTransition* Transition = nullptr);
	bool IsLast() const { return ChildTransitions.Num() == 0; }
	TArray<FString> GetNodeGoals() const;
	void ExecutePreEvents() const;
	bool ContainsTransition(UQuestTransition* Transition) const { return ChildTransitions.Contains(Transition); }

protected:
	UPROPERTY()
	TArray<UQuestTransition*> ChildTransitions;
};

