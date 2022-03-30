// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/TransitionEvents/GiveMoneyScheduledEvent.h"

#include "FAGameMode.h"
#include "Components/GameEconomyComponent.h"

void UGiveMoneyScheduledEvent::Execute_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameEconomyComponent* EconomyComponent =
			Cast<AFAGameMode>(World->GetAuthGameMode())->GetEconomyComponent();
		EconomyComponent->AddPlayerMoneyScheduled(MoneyToGive, TimeToWait);
	}
}

UGiveMoneyScheduledEvent* UGiveMoneyScheduledEvent::CreateGiveMoneyScheduledEvent(
	const int64 Value,
	const float TimeToWait,
	UObject* Outer)
{
	UGiveMoneyScheduledEvent* GiveMoneyScheduledEvent = NewObject<UGiveMoneyScheduledEvent>(Outer);
	GiveMoneyScheduledEvent->MoneyToGive = Value;
	GiveMoneyScheduledEvent->TimeToWait = TimeToWait;
	return GiveMoneyScheduledEvent;
}
