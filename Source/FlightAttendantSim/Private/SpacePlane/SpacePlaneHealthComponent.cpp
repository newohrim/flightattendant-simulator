// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePlane/SpacePlaneHealthComponent.h"


// Sets default values for this component's properties
USpacePlaneHealthComponent::USpacePlaneHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USpacePlaneHealthComponent::AddHealth(float HealthToAdd)
{
	if (HealthToAdd <= 0.0f)
		return;
	Health = FMath::Clamp(Health + HealthToAdd, 0.0f, MaxHealth);
	SpacePlaneReceiveHealth.Broadcast(HealthToAdd);
}

void USpacePlaneHealthComponent::AddDamage(float Damage)
{
	if (Damage <= 0.0f)
		return;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	SpacePlaneReceiveDamage.Broadcast(Damage);
	if (Health == 0.0f)
	{
		SpacePlaneDie.Broadcast();
	}
}

// Called when the game starts
void USpacePlaneHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
