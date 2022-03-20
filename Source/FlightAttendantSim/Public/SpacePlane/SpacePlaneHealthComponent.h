// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpacePlaneHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpacePlaneDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpacePlaneReceiveHealth, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpacePlaneReceiveDamage, float, Damage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API USpacePlaneHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSpacePlaneDie SpacePlaneDie;

	UPROPERTY(BlueprintAssignable)
	FSpacePlaneReceiveHealth SpacePlaneReceiveHealth;
	
	UPROPERTY(BlueprintAssignable)
	FSpacePlaneReceiveDamage SpacePlaneReceiveDamage;
	
	// Sets default values for this component's properties
	USpacePlaneHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const { return MaxHealth; }

	void AddHealth(float HealthToAdd);

	UFUNCTION(BlueprintCallable)
	void AddDamage(float Damage); 

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Health = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 100.0f;
	
	// Called when the game starts
	virtual void BeginPlay() override;
};
