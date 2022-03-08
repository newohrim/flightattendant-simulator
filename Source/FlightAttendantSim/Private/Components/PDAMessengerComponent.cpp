// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PDAMessengerComponent.h"


// Sets default values for this component's properties
UPDAMessengerComponent::UPDAMessengerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPDAMessengerComponent::AddMessage(const FPDAMessage Message)
{
	ReceivedMessages.Add(Message);
	MessageReceived.Broadcast();
}

void UPDAMessengerComponent::AddScheduledMessage(const FPDAMessage& ScheduledMessage, const float TimeToWait)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UPDAMessengerComponent::AddMessage, ScheduledMessage);
		World->GetTimerManager().SetTimer(MessageTimerHandle, TimerDelegate, TimeToWait, false);
	}
}

