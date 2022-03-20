// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CargoDelivery/CargoInfo.h"
#include "Components/ActorComponent.h"
#include "CargoCellComponent.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UCargoCellComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCargoCellComponent();
	
	UFUNCTION(BlueprintCallable)
	int32 GetCargoLoad(const int32 PassengersCount) const;

	bool AddCargo(const FCargoInfo& Cargo, const int32 PassengersCount);

	void RemoveCargo(const FCargoInfo& Cargo);

	int32 GetCapacity() const { return Capacity; }
	
	void SetCapacity(const int32 NewCapacity);

protected:
	constexpr static int32 PassengersCargoWeight = 10;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Capacity = 100;
	UPROPERTY(BlueprintReadOnly)
	TArray<FCargoInfo> TakenCargoes;
};
