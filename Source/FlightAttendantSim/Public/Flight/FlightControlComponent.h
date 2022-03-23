// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlightControlComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UFlightControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFlightControlComponent();

	UFUNCTION(BlueprintCallable)
	void StartFlight();

	UFUNCTION(BlueprintCallable)
	void EndFlight();

protected:
	FTimerHandle FlightTimer;

	UPROPERTY(EditDefaultsOnly, Category = "FlightParams")
	float FlightDuration = 120.0f;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnFlightStart();

	UFUNCTION(BlueprintNativeEvent)
	void OnFlightEnd();
};
