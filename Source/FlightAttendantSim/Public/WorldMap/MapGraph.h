// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
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

	UFUNCTION(BlueprintCallable)
	UMapNode* GetRootNode() const { return RootNode; }
	UFUNCTION(BlueprintCallable)
	UMapNode* GetCurrentNode() const { return CurrentNode; }

protected:
	int32 MaxDepth = 10;
	int32 MaxFacilitiesNum = 5;
	int32 MaxChildrenNodesNum = 5;

	UPROPERTY()
	UMapNode* RootNode;
	UPROPERTY()
	UMapNode* CurrentNode;
};
