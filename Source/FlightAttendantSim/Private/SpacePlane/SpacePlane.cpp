// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePlane/SpacePlane.h"
#include "SpacePlane/PassengerSeat.h"
#include "Characters/FABasePassenger.h"

void USpacePlane::SetUp(const TArray<AActor*>& PassengerSeatsActors)
{
	PassengerSeats.Reserve(PassengerSeatsActors.Num());
	for (int i = 0; i < PassengerSeatsActors.Num(); ++i)
	{
		PassengerSeats.Add(Cast<APassengerSeat>(PassengerSeatsActors[i]));
	}
}

bool USpacePlane::AssignPassenger(AFABasePassenger* PassengerToAssign)
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

void USpacePlane::DeassignPassenger(AFABasePassenger* PassengerToDeassign)
{
	PassengerToDeassign->DeassignPassengerSeat();
	AssignedPassengers.Remove(PassengerToDeassign);
}

APassengerSeat* USpacePlane::GetFreeSeat() const
{
	for (APassengerSeat* PassengerSeat : PassengerSeats)
	{
		if (!PassengerSeat->IsCharacterAssigned())
			return PassengerSeat;
	}

	return nullptr;
}
