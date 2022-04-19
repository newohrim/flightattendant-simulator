// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CargoDelivery/CargoInfo.h"
#include "Components/ActorComponent.h"
#include "CargoManagerComponent.generated.h"

class UMapNode;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UCargoManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCargoManagerComponent();

	// Returns all cargoes for current location
	UFUNCTION(BlueprintCallable)
	const TArray<FCargoInfo>& GetAvailableCargoes() const { return Cargoes; }

	// Generates cargoes for specific node. Clears cached cargoes from previous steps.
	void GenerateCargoes(const UMapNode* NodeFor);

	// In class wrapper for GetDeliveryCost. Need it because structs can't have UFUNCTIONs inside.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int32 CalculateDeliveryCost(const FCargoInfo& Cargo) { return Cargo.GetDeliveryCost(); }

	// Takes cargo delivery contract. Sends the cargo to cargo cell.
	// Returns true if there is enough place in cargo cell.
	UFUNCTION(BlueprintCallable)
	bool TakeCargoDeliveryOffer(const FCargoInfo& Cargo);

	void AddCargoDeliveryOffer(const FCargoInfo& Cargo) { Cargoes.Add(Cargo); }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CargoGeneration")
	int32 MinCargoesCount = 1;
	UPROPERTY(EditDefaultsOnly, Category = "CargoGeneration")
	int32 MaxCargoesCount = 2;
	UPROPERTY(EditDefaultsOnly, Category = "CargoGeneration")
	int32 MinCargoWeight = 10;
	UPROPERTY(EditDefaultsOnly, Category = "CargoGeneration")
	int32 MaxCargoWeight = 50;
	
	// Cargoes available on the current node
	UPROPERTY(BlueprintReadOnly)
	TArray<FCargoInfo> Cargoes;
	
	// Called when the game starts
	virtual void BeginPlay() override;
};
