// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/MapGraph.h"
#include "WorldMap/MapNode.h"

void UMapGraph::GenerateMap(int32 Depth)
{
	this->MaxDepth = Depth;

	RootNode = NewObject<UMapNode>(this);
	RootNode->GenerateChildrenNodes(MaxChildrenNodesNum, MaxFacilitiesNum);
	CurrentNode = RootNode;
}

int32 UMapGraph::GetCurrentDepth() const
{
	return CurrentNode->GetDepth();
}

int32 UMapGraph::GetGraphDepth() const
{
	return RootNode->GetDepth();
}
