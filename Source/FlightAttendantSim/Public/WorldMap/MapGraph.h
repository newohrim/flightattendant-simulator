// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WorldMap/BitmapHeight.h"
#include "MapGraph.generated.h"

class UMapNode;

/**
 * 
 */
UCLASS(BlueprintType)
class FLIGHTATTENDANTSIM_API UMapGraph : public UObject
{
	GENERATED_BODY()
	
public:
	void GenerateMap(int32 Depth);
	int32 GetCurrentDepth() const;
	int32 GetGraphDepth() const;

	UFUNCTION(BlueprintCallable)
	UMapNode* GetRootNode() const { return RootNode; }
	UFUNCTION(BlueprintCallable)
	UMapNode* GetCurrentNode() const { return CurrentNode; }

	UFUNCTION(BlueprintCallable)
	void ExpandNode(UMapNode* Node);

protected:
	int32 MaxDepth = 10;
	int32 MaxFacilitiesNum = 5;
	int32 MaxChildrenNodesNum = 3;

	UPROPERTY()
	UMapNode* RootNode;
	UPROPERTY()
	UMapNode* CurrentNode;

private:
	FBitmapHeight HeightMap;
	TMap<TPair<int32, int32>, UMapNode*> BusyNodes;

	void FixIntersections(const UMapNode* Node);
	void FindIntersections(const UMapNode* Node, TArray<UMapNode*>& PositiveIntersections, TArray<UMapNode*>& NegativeIntersections) const;
	UMapNode* FindCommonAncestor(TArray<UMapNode*> Intersections) const;
};

