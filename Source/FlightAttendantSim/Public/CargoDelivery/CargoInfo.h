// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CargoInfo.generated.h"

class UMapNode;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCargoInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UMapNode> LocationFrom;
	
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UMapNode> LocationTo;
	
	UPROPERTY(BlueprintReadOnly, SaveGame)
	int32 CargoWeight;
	
	UPROPERTY(BlueprintReadOnly, SaveGame)
	FString CompanyName;

	int32 GetDeliveryCost() const { return FMath::Floor(CargoWeight * CostWeightMultiplier); }

	friend bool operator==(const FCargoInfo& Lhs, const FCargoInfo& RHS)
	{
		return Lhs.LocationFrom == RHS.LocationFrom
			&& Lhs.LocationTo == RHS.LocationTo
			&& Lhs.CargoWeight == RHS.CargoWeight
			&& Lhs.CompanyName == RHS.CompanyName;
	}

	friend bool operator!=(const FCargoInfo& Lhs, const FCargoInfo& RHS)
	{
		return !(Lhs == RHS);
	}

private:
	static constexpr float CostWeightMultiplier = 1.0f;
};
