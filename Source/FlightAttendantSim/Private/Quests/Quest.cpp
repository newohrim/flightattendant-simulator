// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/Quest.h"
#include "FAGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Quests/QuestNode.h"

void UQuest::Init_Implementation()
{
	// PLACEHOLDER
}

TArray<FString> UQuest::GetCurrentGoals() const
{
	return CurrentNode->GetNodeGoals();
}

void UQuest::TakeQuest()
{
	if (!CurrentNode || QuestStatus != EQuestStatus::Waiting)
		return;

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(World);
	if (!GameInstance)
	{
		return;
	}
	Cast<UFAGameInstance>(GameInstance)->AddTakenQuest(this);

	CurrentNode->NodeCompleted.BindUObject(this, &UQuest::ChangeNode);
	QuestStatus = EQuestStatus::Taken;
}

void UQuest::ChangeNode(UQuestNode* NextNode)
{
	if (QuestStatus != EQuestStatus::Taken)
		return;
	if (NextNode->IsLast())
		FinishQuest();
	
	CurrentNode->NodeCompleted.Unbind();
	CurrentNode = NextNode;
	CurrentNode->NodeCompleted.BindUObject(this, &UQuest::ChangeNode);
}

void UQuest::FinishQuest()
{
	QuestStatus = EQuestStatus::Completed;
	CurrentNode->NodeCompleted.Unbind();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Quest completed."));
}
