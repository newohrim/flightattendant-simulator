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
	FixIntersections(Node);
}

void UMapGraph::FixIntersections(const UMapNode* Node)
{
	TArray<UMapNode*> PositiveIntersections, NegativeIntersections;
	FindIntersections(Node, PositiveIntersections, NegativeIntersections);
	if (PositiveIntersections.Num() > 0)
	{
		UMapNode* CommonAncestor = FindCommonAncestor(PositiveIntersections);
		CommonAncestor->UpdateHeightLevels(PositiveIntersections.Num());
		// Check for new intersections
		FixIntersections(CommonAncestor);
	}
	if (NegativeIntersections.Num() > 0)
	{
		UMapNode* CommonAncestor = FindCommonAncestor(NegativeIntersections);
		CommonAncestor->UpdateHeightLevels(-NegativeIntersections.Num());
		// Check for new intersections
		FixIntersections(CommonAncestor);
	}
}

void UMapGraph::FindIntersections(const UMapNode* Node, TArray<UMapNode*>& PositiveIntersections,
                                  TArray<UMapNode*>& NegativeIntersections) const
{
	for (UMapNode* Child : Node->GetChildNodes())
	{
		const bool IsBusy = HeightMap.GetBit(Child->GetDepth(), Child->GetHeightLevel());
		if (IsBusy)
		{
			const int32 ParentHeight =
				BusyNodes[TPair<int32, int32>(Child->GetDepth(), Child->GetHeightLevel())]->GetParentNode()->GetHeightLevel();
			if (ParentHeight > Node->GetHeightLevel())
			{
				PositiveIntersections.Add(Child);
			}
			else
			{
				NegativeIntersections.Add(Child);
			}
		}
	}
}

UMapNode* UMapGraph::FindCommonAncestor(TArray<UMapNode*> Intersections) const
{
	if (Intersections.Num() == 1)
		return Intersections[0]->GetParentNode();
	
	while (true)
	{
		bool Found = true;
		UMapNode* CommonAncestor = Intersections[0]->GetParentNode();
		Intersections[0] = Intersections[0]->GetParentNode();
		for (int i = 1; i < Intersections.Num(); ++i)
		{
			Intersections[i] = Intersections[i]->GetParentNode();
			if (Intersections[i] != CommonAncestor)
				Found = false;
		}
		if (Found)
			return CommonAncestor;
	}
}
