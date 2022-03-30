// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameEconomyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoneyReceived, int64, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoneyLost, int64, Value);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UGameEconomyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FMoneyReceived MoneyReceived;
	UPROPERTY(BlueprintAssignable)
	FMoneyLost MoneyLost;
	
	// Sets default values for this component's properties
	UGameEconomyComponent();
	
	int64 GetPlayerMoney() const { return PlayerMoney; }

	UFUNCTION(BlueprintCallable)
	void AddPlayerMoney(const int64 Value);

	UFUNCTION(BlueprintCallable)
	void AddPlayerMoneyScheduled(const int64 Value, const float TimeToWait);

	UFUNCTION(BlueprintCallable)
	void WithdrawPlayerMoney(const int64 Value);

	UFUNCTION(BlueprintCallable)
	void WithdrawPlayerMoneyScheduled(const int64 Value, const float TimeToWait);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int64 PlayerMoney = 0;

	FTimerHandle EconomyTimerHandle;;
};
