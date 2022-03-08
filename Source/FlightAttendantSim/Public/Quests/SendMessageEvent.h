// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PDAMessage.h"
#include "Quests/TransitionEvent.h"
#include "SendMessageEvent.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API USendMessageEvent : public UTransitionEvent
{
	GENERATED_BODY()

public:
	virtual void Execute() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static USendMessageEvent* CreateSendMessageEvent(
		const FPDAMessage& MessageToSend,
		UObject* Outer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPDAMessage Message;
};
