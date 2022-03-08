// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePlane/CargoCell.h"

int32 UCargoCell::GetCargoLoad(const int32 PassengersCount) const
{
	int32 CargoLoad = PassengersCount * PassengersCargoWeight;
	for (const FCargoInfo& Cargo : TakenCargoes)
	{
		CargoLoad += Cargo.CargoWeight;
	}
	return CargoLoad;
}

bool UCargoCell::AddCargo(const FCargoInfo& Cargo, const int32 PassengersCount)
{
	const int32 CurrentLoad = GetCargoLoad(PassengersCount);
	if (CurrentLoad + Cargo.CargoWeight <= Capacity)
	{
		TakenCargoes.Add(Cargo);
		return true;
	}
	return false;
}

void UCargoCell::RemoveCargo(const FCargoInfo& Cargo)
{
	TakenCargoes.Remove(Cargo);
}

void UCargoCell::SetCapacity(const int32 NewCapacity)
{
	if (NewCapacity < 0)
	{
		UE_LOG(LogTemp, Error,
			TEXT("CargoCell new capacity was negative %d."), NewCapacity);
		return;
	}
	Capacity = NewCapacity;
}
