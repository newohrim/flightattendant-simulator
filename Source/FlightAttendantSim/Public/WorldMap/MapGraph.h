// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WorldMap/BitmapHeight.h"
#include "MapGraph.generated.h"

class UMapNode;
class UQuest;

typedef TIndexedContainerIterator<TArray<UQuest*>, UQuest*, TArray<UQuest*>::SizeType> TQuestIterator;

/**
 * 
 */
UCLASS(BlueprintType)
class FLIGHTATTENDANTSIM_API UMapGraph : public UObject
{
	GENERATED_BODY()
	
public:
	void GenerateMap(int32 Depth, const TArray<UQuest*>& QuestsToPlace);
	int32 GetCurrentDepth() const;
	int32 GetGraphDepth() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentMaxDepth() const { return CurrentMaxDepth; }
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentMaxHeight() const { return CurrentMaxHeight; }
	UFUNCTION(BlueprintCallable)
	UMapNode* GetRootNode() const { return RootNode; }
	UFUNCTION(BlueprintCallable)
	UMapNode* GetCurrentNode() const { return CurrentNode; }
	void SetCurrentNode(UMapNode* MapNode) { CurrentNode = MapNode; }
	UFUNCTION(BlueprintCallable)
	const TArray<FVector2D>& GetNodesPairs() const { return NodesPairs; }

	//UFUNCTION(BlueprintCallable)
	void ExpandNode(UMapNode* Node, const TArray<UQuest*>& QuestsToPlace);

protected:
	int32 MaxDepth = 10;
	int32 MaxFacilitiesNum = 5;
	int32 MaxChildrenNodesNum = 3;

	UPROPERTY()
	UMapNode* RootNode;
	UPROPERTY()
	UMapNode* CurrentNode;

private:
	int32 CurrentMaxDepth = 0;
	int32 CurrentMaxHeight = 0;
    	
	TArray<FVector2D> NodesPairs;
};

