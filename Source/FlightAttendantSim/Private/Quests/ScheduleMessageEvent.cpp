// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/ScheduleMessageEvent.h"

#include "FAGameMode.h"
#include "Components/PDAMessengerComponent.h"

void UScheduleMessageEvent::Execute_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UPDAMessengerComponent* PDAMessenger =
			Cast<AFAGameMode>(World->GetAuthGameMode())->GetPDAMessenger();
		PDAMessenger->AddScheduledMessage(Message, TimeToWait);
	}
}

UScheduleMessageEvent* UScheduleMessageEvent::CreateScheduleMessageEvent(
	const FPDAMessage& MessageToSend,
	const float TimeToWait,
	UObject* Outer)
{
	UScheduleMessageEvent* Event = NewObject<UScheduleMessageEvent>(Outer);
	Event->Message = MessageToSend;
	Event->TimeToWait = TimeToWait;
	return Event;
}
