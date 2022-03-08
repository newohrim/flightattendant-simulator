// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestTransition.generated.h"

class UTransitionEvent;
class UQuestNode;

class UQuestTransition;
DECLARE_DELEGATE_OneParam(FTransitionCompleted, UQuestTransition*)

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

	UFUNCTION(BlueprintCallable)
	void AddPreEvent(UTransitionEvent* Event) { PreEvents.Add(Event); }
	UFUNCTION(BlueprintCallable)
	void AddPostEvent(UTransitionEvent* Event) { PostEvents.Add(Event); }
	
	void ExecutePreEvents() const;
	void ExecutePostEvents() const;

protected:
	UPROPERTY()
	UQuestNode* TargetNode;
	UPROPERTY(Instanced, EditDefaultsOnly)
	TArray<UTransitionEvent*> PreEvents;
	UPROPERTY(Instanced, EditDefaultsOnly)
	TArray<UTransitionEvent*> PostEvents;

	FString TransitionDescription;
};
