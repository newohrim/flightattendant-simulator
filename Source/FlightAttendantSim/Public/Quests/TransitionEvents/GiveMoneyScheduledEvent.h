// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quests/TransitionEvents/GiveMoneyEvent.h"
#include "GiveMoneyScheduledEvent.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UGiveMoneyScheduledEvent : public UGiveMoneyEvent
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UGiveMoneyScheduledEvent* CreateGiveMoneyScheduledEvent(
		const int64 Value,
		const float TimeToWait,
		UObject* Outer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeToWait;
};
