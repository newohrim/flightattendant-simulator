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
	int32 GetNodeIndex() const { return NodeIndex; }
	void SetNodeIndex(const int32 Index) { if (Index >= 0) NodeIndex = Index; }

protected:
	UPROPERTY()
	TArray<UQuestTransition*> ChildTransitions;

	// A unique node identifier. Gets its true value when created inside quest.
	// Declared to remember quest's progress as a num pointer to the current node.
	// If the game gets updated, new nodes might start index counting from 1000 (2000 for the next update...).
	// It allows me to prevent active game walkthroughs from getting ruined.
	int32 NodeIndex = -1;
};

