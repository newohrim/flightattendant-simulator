// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestTransition.generated.h"

class UTransitionEvent;
class UQuestNode;

DECLARE_DELEGATE_OneParam(FTransitionCompleted, UQuestNode*)

/**
 * 
 */
UCLASS(BlueprintType)
class FLIGHTATTENDANTSIM_API UQuestTransition : public UObject
{
	GENERATED_BODY()

public:
	FTransitionCompleted TransitionComplete;
	
	UFUNCTION(BlueprintCallable)
	UQuestNode* GetTargetNode() const { return TargetNode; }
	UFUNCTION(BlueprintCallable)
	virtual FString GetTransitionDescription() const { return TransitionDescription; }
	UFUNCTION(BlueprintCallable)
	void SetTransitionDescription( const FString& NewDescription ) { TransitionDescription = NewDescription; }
	UFUNCTION(BlueprintCallable)
	virtual void ExecuteTransition();
	UFUNCTION(BlueprintCallable)
	void SetTargetNode(UQuestNode* Node) { TargetNode = Node; }

protected:
	UPROPERTY()
	UQuestNode* TargetNode;
	TSubclassOf<UTransitionEvent> PreEvent;
	TSubclassOf<UTransitionEvent> PostEvent;

	FString TransitionDescription;
};
