// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlightControlComponent.generated.h"

class UMapNode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerArrived, UMapNode*, Destination);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerStartTravel, UMapNode*, Destination);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UFlightControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FPlayerArrived PlayerArrived;

	FPlayerStartTravel PlayerStartTravel;
	
	// Sets default values for this component's properties
	UFlightControlComponent();

	// Start flight to destination
	UFUNCTION(BlueprintCallable)
	void StartFlight(UMapNode* Destination);

	// Is called by a timer to stop the current flight and arrive to destination
	UFUNCTION(BlueprintCallable)
	void EndFlight();

protected:
	FTimerHandle FlightTimer;

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UMapNode> DestinationNode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlightParams")
	float FlightDuration = 120.0f;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnFlightStart();

	UFUNCTION(BlueprintNativeEvent)
	void OnFlightEnd();
};
