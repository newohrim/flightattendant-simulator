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
	const int32 OriginalNodeDepth = Node->GetDepth();
	Node->GenerateChildrenNodes(MaxChildrenNodesNum, MaxFacilitiesNum, OriginalNodeDepth + 1);

	auto& ChildrenNodes = Node->GetChildNodes();
	const int32 ChildCount = ChildrenNodes.Num();
	const int32 OriginalNodeHeight = Node->GetHeightLevel();
	const int32 MidIndex = ChildCount / 2;
	const int32 OddOffset = ChildCount & 1;
	// HeightLevels array is needed to search for intersections in HeightMap later
	TArray<int32> HeightLevels;
	if (OddOffset == 1)
	{
		// The middle node is always on the same level as the original one
		ChildrenNodes[MidIndex]->SetHeightLevel(OriginalNodeHeight);
		HeightLevels.Add(OriginalNodeHeight);
	}
	for (int32 i = MidIndex - 1; i >= 0; --i)
	{
		// Positive levels relative to the original node
		int32 HeightLevel = OriginalNodeHeight + MidIndex - i;
		ChildrenNodes[i]->SetHeightLevel(HeightLevel);
		HeightLevels.Add(HeightLevel);
	}
	for (int32 i = MidIndex + OddOffset; i < ChildCount; ++i)
	{
		// Negative levels relative to the original node
		int32 HeightLevel = OriginalNodeHeight + MidIndex - i - (OddOffset ^ 1);
		ChildrenNodes[i]->SetHeightLevel(HeightLevel);
		HeightLevels.Add(HeightLevel);
	}

	if (Node != RootNode)
		Node->GetParentNode()->FixIntersections(HeightLevels, Node);

	/*
	const UMapNode* ParentNode = Node->GetParentNode();
	auto& ParentNodeChildren = ParentNode->GetChildNodes();
	int32 HigherNeighbourIndex = -1;
	int32 LowerNeighbourIndex = ParentNodeChildren.Num();
	// Height may change, so I need to iterate through all column to find neighbours of original node
	for (int32 i = 0; i < ParentNodeChildren.Num(); ++i)
	{
		if (ParentNodeChildren[i] == Node)
		{
			HigherNeighbourIndex = i - 1;
			LowerNeighbourIndex = i + 1;
			break;
		}
	}
	
	if (HigherNeighbourIndex >= 0)
	{
		int32 PositiveIntersectionsCount = 0;
		for (UMapNode* ChildN : ParentNodeChildren[HigherNeighbourIndex])
		{
			for (int32 Height : HeightLevels)
			{
				if (ChildN->GetHeightLevel() == Height)
				{
					++PositiveIntersectionsCount;
					break;
				}
			}
		}
		ParentNodeChildren[HigherNeighbourIndex]->UpdateHeightLevels(PositiveIntersectionsCount);
	}
	if (LowerNeighbourIndex < ParentNodeChildren.Num())
	{
		int32 NegativeIntersectionsCount = 0;
		for (UMapNode* ChildN : ParentNodeChildren[LowerNeighbourIndex])
		{
			for (int32 Height : HeightLevels)
			{
				if (ChildN->GetHeightLevel() == Height)
				{
					++NegativeIntersectionsCount;
					break;
				}
			}
		}
		ParentNodeChildren[LowerNeighbourIndex]->UpdateHeightLevels(NegativeIntersectionsCount);
	}*/
}
