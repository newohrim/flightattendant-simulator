// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MapNode.generated.h"

UENUM(BlueprintType)
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
	int32 GetHeightLevel() const { return HeightLevel; }
	UFUNCTION(BlueprintCallable)
	void SetHeightLevel(const int32 NewLevel) { HeightLevel = NewLevel; }
	UFUNCTION(BlueprintCallable)
	int32 GetGraphDepth() const;
	UFUNCTION(BlueprintCallable)
	int32 GetDepth() const { return Depth; }
	UFUNCTION(BlueprintCallable)
	void SetDepth(const int32 NewDepth) { Depth = NewDepth; }
	UFUNCTION(BlueprintCallable)
	UMapNode* GetParentNode() const { return ParentNode; }
	UFUNCTION(BlueprintCallable)
	void SetParentNode(UMapNode* NewParentNode) { ParentNode = NewParentNode; }

	void UpdateHeightLevels(int32 HeightIncrement);
	void MakeGridLayout(int32& X);
	void GetConnectedPairs(TArray<FVector2D>& NodePairs) const;
	
	UFUNCTION(BlueprintCallable)
	void GenerateChildrenNodes(const int32 Num, const int32 MaxFacilitiesNum, const int32 NewDepth);
	UFUNCTION(BlueprintCallable)
	const TArray<UMapNode*>& GetChildNodes() const { return ChildNodes; }

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<UMapNode*> ChildNodes;
	UPROPERTY(BlueprintReadOnly)
	TArray<TEnumAsByte<EFacilityType>> Facilities;
	UPROPERTY(BlueprintReadOnly)
	UMapNode* ParentNode = nullptr;

	UMapNode* GenerateNode(int32 FacilitiesNum);

private:
	int32 HeightLevel = 0;
	int32 Depth = 0;
};
