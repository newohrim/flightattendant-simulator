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

void UQuestNode::CompleteNode(UQuestNode* NextNode)
{
	NodeCompleted.ExecuteIfBound(NextNode);
}

void UQuestNode::AddTransition(UQuestTransition* Transition)
{
	ChildTransitions.Add(Transition);
	ChildTransitions.Last()->TransitionComplete.BindUObject(this, &UQuestNode::CompleteNode);
}

UQuestTransition* UQuestNode::CreateTransition()
{
	return NewObject<UQuestTransition>(this);
}
