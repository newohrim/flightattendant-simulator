// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestNode.h"
#include "Quests/QuestTransition.h"

void UQuestNode::Init()
{
	for (UQuestTransition* Transition : ChildTransitions)
	{
		Transition->TransitionComplete.BindUObject(this, &UQuestNode::CompleteNode);
	}
}

void UQuestNode::CompleteNode(UQuestTransition* ExecutedTransition)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
	const bool Success = NodeCompleted.ExecuteIfBound(ExecutedTransition);
	UE_LOG(LogTemp, Warning, TEXT("%d"), static_cast<int32>(Success));
}

UQuestTransition* UQuestNode::AddTransition(const FString& QuestDescription, UQuestTransition* Transition)
{
	if (!Transition)
		Transition = NewObject<UQuestTransition>(this);
	ChildTransitions.Add(Transition);
	UQuestTransition* AddedTransition = ChildTransitions.Last();
	AddedTransition->SetTransitionDescription(QuestDescription);
	AddedTransition->TransitionComplete.BindUObject(this, &UQuestNode::CompleteNode);
	
	return Transition;
}

TArray<FString> UQuestNode::GetNodeGoals() const
{
	TArray<FString> Goals;
	for (const UQuestTransition* Transition : ChildTransitions)
	{
		Goals.Add(Transition->GetTransitionDescription());
	}
	
	return Goals;
}

void UQuestNode::ExecutePreEvents() const
{
	for (const UQuestTransition* Transition : ChildTransitions)
	{
		Transition->ExecutePreEvents();
	}
}
