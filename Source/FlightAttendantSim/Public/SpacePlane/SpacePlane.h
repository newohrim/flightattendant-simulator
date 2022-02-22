// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SpacePlane.generated.h"

class APassengerSeat;
class AFABasePassenger;

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API USpacePlane : public UObject
{
	GENERATED_BODY()

public:
	void SetUp(const TArray<AActor*>& PassengerSeatsActors);

	bool AssignPassenger(AFABasePassenger* PassengerToAssign);
	void DeassignPassenger(AFABasePassenger* PassengerToDeassign);
	
	bool HasPlace() const { return AssignedPassengers.Num() < PassengerSeats.Num(); }
	int32 GetSeatsNum() const { return PassengerSeats.Num(); }

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<APassengerSeat*> PassengerSeats;
	UPROPERTY(BlueprintReadOnly)
	TArray<AFABasePassenger*> AssignedPassengers;

	APassengerSeat* GetFreeSeat() const;
};
