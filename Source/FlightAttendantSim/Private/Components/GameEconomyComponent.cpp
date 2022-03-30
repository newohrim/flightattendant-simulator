// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GameEconomyComponent.h"

// Sets default values for this component's properties
UGameEconomyComponent::UGameEconomyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UGameEconomyComponent::AddPlayerMoney(const int64 Value)
{
	if (Value > 0)
	{
		const int64 ClampedVal =
			FMath::Clamp(Value, 0LL, INT64_MAX - PlayerMoney);
		PlayerMoney += ClampedVal;
		MoneyReceived.Broadcast(ClampedVal);
	}
}

void UGameEconomyComponent::AddPlayerMoneyScheduled(const int64 Value, const float TimeToWait)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UGameEconomyComponent::AddPlayerMoney, Value);
		World->GetTimerManager().SetTimer(EconomyTimerHandle, TimerDelegate, TimeToWait, false);
	}
}

void UGameEconomyComponent::WithdrawPlayerMoney(const int64 Value)
{
	if (Value > 0)
	{
		PlayerMoney = FMath::Clamp(PlayerMoney - Value, 0LL, INT64_MAX);
		MoneyLost.Broadcast(Value);
	}
}

void UGameEconomyComponent::WithdrawPlayerMoneyScheduled(const int64 Value, const float TimeToWait)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UGameEconomyComponent::WithdrawPlayerMoney, Value);
		World->GetTimerManager().SetTimer(EconomyTimerHandle, TimerDelegate, TimeToWait, false);
	}
}

