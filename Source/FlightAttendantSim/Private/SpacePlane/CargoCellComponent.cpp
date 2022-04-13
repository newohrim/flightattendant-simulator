// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePlane/CargoCellComponent.h"

#include "FAGameMode.h"
#include "Flight/FlightControlComponent.h"
#include "Components/GameEconomyComponent.h"

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
		CargoLoadChanged.Broadcast();
		return true;
	}
	return false;
}

void UCargoCellComponent::RemoveCargo(const FCargoInfo& Cargo)
{
	TakenCargoes.Remove(Cargo);
	CargoLoadChanged.Broadcast();
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

void UCargoCellComponent::BeginPlay()
{
	Super::BeginPlay();

	UFlightControlComponent* FlightController = FAGAMEMODE->GetFlightController();
	FlightController->PlayerArrived.AddDynamic(this, &UCargoCellComponent::PlayerArrivedHandle);
}

void UCargoCellComponent::PlayerArrivedHandle(UMapNode* Destination)
{
	int64 Sum = 0;
	for (int32 i = TakenCargoes.Num() - 1; i >= 0; --i)
	{
		if (TakenCargoes[i].LocationTo.Get() == Destination)
		{
			Sum += TakenCargoes[i].GetDeliveryCost();
			TakenCargoes.RemoveAt(i);
		}
	}
	if (Sum > 0)
	{
		UGameEconomyComponent* EconomyComponent = FAGAMEMODE->GetEconomyComponent();
		EconomyComponent->AddPlayerMoney(Sum);
		CargoLoadChanged.Broadcast();
	}
}
