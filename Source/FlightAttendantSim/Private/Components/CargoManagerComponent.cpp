// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CargoManagerComponent.h"

#include "WorldMap/MapNode.h"
#include "SpacePlane/CargoCellComponent.h"
#include "FAGameMode.h"
#include "SpacePlane/SpacePlaneComponent.h"


// Sets default values for this component's properties
UCargoManagerComponent::UCargoManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCargoManagerComponent::GenerateCargoes(const UMapNode* NodeFor)
{
	if (!NodeFor)
		return;
	
	Cargoes.Empty();
	auto& ChildNodes = NodeFor->GetChildNodes();
	for (const UMapNode* DestinationNode : ChildNodes)
	{
		const int32 CargoesNum = FMath::RandRange(MinCargoesCount, MaxCargoesCount);
		for (int i = 0; i < CargoesNum; ++i)
		{
			const int32 Weight = FMath::RandRange(MinCargoWeight, MaxCargoWeight);
			Cargoes.Add(
				{
					NodeFor,
					DestinationNode,
					Weight,
					"DebugCompany"
				});
		}
	}
}

bool UCargoManagerComponent::TakeCargoDeliveryOffer(const FCargoInfo& Cargo)
{
	UWorld* World = GetWorld();
	if (World)
	{
		USpacePlaneComponent* SpacePlane =
			Cast<AFAGameMode>(World->GetAuthGameMode())->GetSpacePlane();
		if (SpacePlane)
		{
			UCargoCellComponent* CargoCell = SpacePlane->GetCargoCell();
			if (CargoCell)
			{
				const bool Success =
					CargoCell->AddCargo(Cargo, SpacePlane->GetPassengersCount());
				if (Success)
				{
					Cargoes.Remove(Cargo);
					return true;
				}
				return false;
			}
		}
	}

	UE_LOG(LogTemp, Error,
		TEXT("SpacePlane or its CargoCell were not initialized correctly."));
	return false;
}


// Called when the game starts
void UCargoManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

