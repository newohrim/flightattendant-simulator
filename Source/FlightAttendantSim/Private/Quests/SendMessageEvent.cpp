// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/SendMessageEvent.h"

#include "FAGameMode.h"
#include "Components/PDAMessengerComponent.h"

void USendMessageEvent::Execute()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UPDAMessengerComponent* PDAMessenger =
			Cast<AFAGameMode>(World->GetAuthGameMode())->GetPDAMessenger();
		PDAMessenger->AddMessage(Message);
	}
}

USendMessageEvent* USendMessageEvent::CreateSendMessageEvent(const FPDAMessage& MessageToSend, UObject* Outer)
{
	USendMessageEvent* Event = NewObject<USendMessageEvent>(Outer);
	Event->Message = MessageToSend;
	return Event;
}
