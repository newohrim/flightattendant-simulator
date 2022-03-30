// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PDAMessage.h"
#include "Components/ActorComponent.h"
#include "PDAMessengerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageReceived, const FPDAMessage&, RecievedMessage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UPDAMessengerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FMessageReceived MessageReceived;
	
	// Sets default values for this component's properties
	UPDAMessengerComponent();

	UFUNCTION(BlueprintCallable)
	void AddMessage(const FPDAMessage Message);

	UFUNCTION(BlueprintCallable)
	void AddScheduledMessage(const FPDAMessage& ScheduledMessage, const float TimeToWait);

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FPDAMessage> ReceivedMessages;

	FTimerHandle MessageTimerHandle;
};
