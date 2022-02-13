// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/GiveQuestEvent.h"
#include "Quests/Quest.h"

void UGiveQuestEvent::EnterEvent_Implementation(UDlgContext* Context, UObject* Participant)
{
	Super::EnterEvent_Implementation(Context, Participant);

	if (!QuestToGive)
	{
		UE_LOG(LogTemp, Display, TEXT("QuestToGive in dialogue event wans't set properly."));
		return;
	}
	// QuestToGive->Init();
	// TODO: Quest properties are need to be set and Init() to be called before TakeQuest.
	QuestToGive->TakeQuest();
}
