// Fill out your copyright notice in the Description page of Project Settings.


#include "Flight/FlightControlComponent.h"


// Sets default values for this component's properties
UFlightControlComponent::UFlightControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UFlightControlComponent::StartFlight()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerDelegate FlightTimerDel;
		FlightTimerDel.BindUObject(this, &UFlightControlComponent::EndFlight);
		World->GetTimerManager().SetTimer(FlightTimer, FlightTimerDel, FlightDuration, false);
		
		OnFlightStart();
	}
}

void UFlightControlComponent::EndFlight()
{
	// passengers leave plane?
	
	OnFlightEnd();
}


// Called when the game starts
void UFlightControlComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UFlightControlComponent::OnFlightStart_Implementation()
{
	// PLACEHOLDER
}

void UFlightControlComponent::OnFlightEnd_Implementation()
{
	// PLACEHOLDER
}

