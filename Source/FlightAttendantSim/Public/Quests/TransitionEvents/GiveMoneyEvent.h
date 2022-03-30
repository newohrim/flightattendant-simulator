// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quests/TransitionEvent.h"
#include "GiveMoneyEvent.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UGiveMoneyEvent : public UTransitionEvent
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UGiveMoneyEvent* CreateGiveMoneyEvent(const int64 Value, UObject* Outer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int64 MoneyToGive = 0;
};
