// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpacePlaneComponent.generated.h"

class USpacePlaneHealthComponent;
class UCargoCellComponent;
class APassengerSeat;
class AFABasePassenger;

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API USpacePlaneComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpacePlaneComponent();
	
	void SetUp(const TArray<AActor*>& PassengerSeatsActors);

	bool AssignPassenger(AFABasePassenger* PassengerToAssign);
	void DeassignPassenger(AFABasePassenger* PassengerToDeassign);
	
	bool HasPlace() const { return AssignedPassengers.Num() < PassengerSeats.Num(); }
	int32 GetSeatsNum() const { return PassengerSeats.Num(); }
	UFUNCTION(BlueprintCallable)
	int32 GetPassengersCount() const { return AssignedPassengers.Num(); }

	USpacePlaneHealthComponent* GetSpacePlaneHealth() const { return HealthComponent; }

	UCargoCellComponent* GetCargoCell() const { return CargoCell; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpacePlaneHealthComponent* HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCargoCellComponent* CargoCell;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<APassengerSeat*> PassengerSeats;
	UPROPERTY(BlueprintReadOnly)
	TArray<AFABasePassenger*> AssignedPassengers;

	APassengerSeat* GetFreeSeat() const;
};
