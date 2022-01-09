// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/QuestTransition.h"
#include "Quests/TransitionEvent.h"
#include "Quests/QuestNode.h"

void UQuestTransition::ExecuteTransition()
{
	TransitionComplete.ExecuteIfBound(TargetNode);
}
