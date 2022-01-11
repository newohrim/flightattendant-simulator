// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MapNode.generated.h"

UENUM()
enum EFacilityType
{
	Shop			UMETA(DisplayName = "Shop"),
	BountyHunter	UMETA(DisplayName = "BountyHunter"),
	CargoShipping	UMETA(DisplayName = "CargoShipping"),
	RepairStation	UMETA(DisplayName = "RepairStation"),
	RouteGuide		UMETA(DisplayName = "RouteGuide"),

	// Last element to always know how many different enum values are possible
	FACILITY_NR_ITEMS
};

/**
 * 
 */
UCLASS(BlueprintType)
class FLIGHTATTENDANTSIM_API UMapNode : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int32 GetDepth() const;
	UFUNCTION(BlueprintCallable)
	void GenerateChildrenNodes(const int32 Num, const int32 MaxFacilitiesNum);
	UFUNCTION(BlueprintCallable)
	const TArray<UMapNode*>& GetChildNodes() const { return ChildNodes; }

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UMapNode*> ChildNodes;
	UPROPERTY(BlueprintReadOnly)
	TArray<TEnumAsByte<EFacilityType>> Facilities;

	UMapNode* GenerateNode(int32 FacilitiesNum);
};
