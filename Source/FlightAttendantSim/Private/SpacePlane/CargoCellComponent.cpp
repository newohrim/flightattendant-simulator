// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePlane/CargoCellComponent.h"

UCargoCellComponent::UCargoCellComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UCargoCellComponent::GetCargoLoad(const int32 PassengersCount) const
{
	int32 CargoLoad = PassengersCount * PassengersCargoWeight;
	for (const FCargoInfo& Cargo : TakenCargoes)
	{
		CargoLoad += Cargo.CargoWeight;
	}
	return CargoLoad;
}

bool UCargoCellComponent::AddCargo(const FCargoInfo& Cargo, const int32 PassengersCount)
{
	const int32 CurrentLoad = GetCargoLoad(PassengersCount);
	if (CurrentLoad + Cargo.CargoWeight <= Capacity)
	{
		TakenCargoes.Add(Cargo);
		return true;
	}
	return false;
}

void UCargoCellComponent::RemoveCargo(const FCargoInfo& Cargo)
{
	TakenCargoes.Remove(Cargo);
}

void UCargoCellComponent::SetCapacity(const int32 NewCapacity)
{
	if (NewCapacity < 0)
	{
		UE_LOG(LogTemp, Error,
			TEXT("CargoCell new capacity was negative %d."), NewCapacity);
		return;
	}
	Capacity = NewCapacity;
}
