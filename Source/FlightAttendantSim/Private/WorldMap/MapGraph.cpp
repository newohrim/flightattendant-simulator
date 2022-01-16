// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/MapGraph.h"
#include "WorldMap/MapNode.h"

void UMapGraph::GenerateMap(int32 Depth)
{
	this->MaxDepth = Depth;

	RootNode = NewObject<UMapNode>(this);
	RootNode->SetHeightLevel(0);
	ExpandNode(RootNode);
	//RootNode->GenerateChildrenNodes(MaxChildrenNodesNum, MaxFacilitiesNum);
	CurrentNode = RootNode;
}

int32 UMapGraph::GetCurrentDepth() const
{
	return CurrentNode->GetDepth();
}

int32 UMapGraph::GetGraphDepth() const
{
	return RootNode->GetGraphDepth();
}

void UMapGraph::ExpandNode(UMapNode* Node)
{
	if (Node->GetChildNodes().Num() > 0)
		return;

	Node->GenerateChildrenNodes(MaxChildrenNodesNum, MaxFacilitiesNum, Node->GetDepth() + 1);
	//FixIntersections(Node);
	int32 X = 0;
	RootNode->MakeGridLayout(X);
	// Max height is in X after MakeGridLayout call, so I make a half offset
	CurrentMaxHeight = X / 2;
	CurrentMaxDepth = GetGraphDepth();
	RootNode->UpdateHeightLevels(-CurrentMaxHeight - (RootNode->GetHeightLevel() - CurrentMaxHeight));
	NodesPairs.Empty();
	RootNode->GetConnectedPairs(NodesPairs);
}
