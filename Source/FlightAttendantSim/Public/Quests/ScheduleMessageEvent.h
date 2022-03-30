// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SendMessageEvent.h"
#include "ScheduleMessageEvent.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UScheduleMessageEvent : public USendMessageEvent
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UScheduleMessageEvent* CreateScheduleMessageEvent(
		const FPDAMessage& MessageToSend,
		const float TimeToWait,
		UObject* Outer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeToWait;
};
