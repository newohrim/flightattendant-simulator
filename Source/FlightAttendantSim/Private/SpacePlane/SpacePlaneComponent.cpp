// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePlane/SpacePlaneComponent.h"
#include "SpacePlane/PassengerSeat.h"
#include "Characters/FABasePassenger.h"
#include "SpacePlane/CargoCellComponent.h"
#include "SpacePlane/SpacePlaneHealthComponent.h"

USpacePlaneComponent::USpacePlaneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HealthComponent =
		CreateDefaultSubobject<USpacePlaneHealthComponent>("HealthComponent");
	CargoCell =
		CreateDefaultSubobject<UCargoCellComponent>("CargoCellComponent");
}

void USpacePlaneComponent::SetUp(const TArray<AActor*>& PassengerSeatsActors)
{
	PassengerSeats.Reserve(PassengerSeatsActors.Num());
	for (int i = 0; i < PassengerSeatsActors.Num(); ++i)
	{
		PassengerSeats.Add(Cast<APassengerSeat>(PassengerSeatsActors[i]));
	}
}

bool USpacePlaneComponent::AssignPassenger(AFABasePassenger* PassengerToAssign)
{
	if (!HasPlace())
		return false;

	APassengerSeat* FreeSeat = GetFreeSeat();
	if (FreeSeat)
	{
		PassengerToAssign->AssignPassengerSeat(FreeSeat);
		AssignedPassengers.Add(PassengerToAssign);
		return true;
	}

	return false;
}

void USpacePlaneComponent::DeassignPassenger(AFABasePassenger* PassengerToDeassign)
{
	PassengerToDeassign->DeassignPassengerSeat();
	AssignedPassengers.Remove(PassengerToDeassign);
}

APassengerSeat* USpacePlaneComponent::GetFreeSeat() const
{
	for (APassengerSeat* PassengerSeat : PassengerSeats)
	{
		if (!PassengerSeat->IsCharacterAssigned())
			return PassengerSeat;
	}

	return nullptr;
}
