// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/TransitionEvents/GiveMoneyEvent.h"

#include "FAGameMode.h"
#include "Components/GameEconomyComponent.h"

void UGiveMoneyEvent::Execute_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameEconomyComponent* EconomyComponent =
			Cast<AFAGameMode>(World->GetAuthGameMode())->GetEconomyComponent();
		EconomyComponent->AddPlayerMoney(MoneyToGive);
	}
}

UGiveMoneyEvent* UGiveMoneyEvent::CreateGiveMoneyEvent(const int64 Value, UObject* Outer)
{
	UGiveMoneyEvent* GiveMoneyEvent = NewObject<UGiveMoneyEvent>(Outer);
	GiveMoneyEvent->MoneyToGive = Value;
	return GiveMoneyEvent;
}
