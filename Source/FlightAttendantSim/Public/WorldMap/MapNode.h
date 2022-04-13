// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MapNode.generated.h"

class ULocationInfo;
class UQuest;

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
	int32 GetGraphMaxAbsHeight() const;
	UFUNCTION(BlueprintCallable)
	int32 GetDepth() const { return Depth; }
	UFUNCTION(BlueprintCallable)
	void SetDepth(const int32 NewDepth) { Depth = NewDepth; }
	UFUNCTION(BlueprintCallable)
	UMapNode* GetParentNode() const { return ParentNode; }
	UFUNCTION(BlueprintCallable)
	void SetParentNode(UMapNode* NewParentNode) { ParentNode = NewParentNode; }
	ULocationInfo* GetLocationInfo() const { return LocationInfo; }
	void SetLocationInfo(ULocationInfo* LocInfo) { LocationInfo = LocInfo; }

	void UpdateHeightLevels(int32 HeightIncrement);
	void MakeGridLayout(int32& X);
	void GetConnectedPairs(TArray<FVector2D>& NodePairs) const;
	
	void GenerateChildrenNodes(const TArray<UQuest*>& QuestsToPlace, const int32 NewDepth);
	//UMapNode* AddChildNode() { ChildNodes.Add(); }
	UFUNCTION(BlueprintCallable)
	const TArray<UMapNode*>& GetChildNodes() const { return ChildNodes; }

protected:
	UPROPERTY(BlueprintReadOnly)
	ULocationInfo* LocationInfo;
	UPROPERTY(BlueprintReadOnly)
	TArray<UMapNode*> ChildNodes;
	UPROPERTY(BlueprintReadOnly)
	UMapNode* ParentNode = nullptr;

	UMapNode* GenerateNode(ULocationInfo* LocInfo);

private:
	static constexpr int32 MaxChildrenCount = 3;
	
	int32 HeightLevel = 0;
	int32 Depth = 0;
};
