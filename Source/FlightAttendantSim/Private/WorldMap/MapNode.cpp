// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/MapNode.h"

int32 UMapNode::GetGraphDepth() const
{
	if (ChildNodes.Num() == 0)
		return 1;

	int32 MaxDepth = 1;
	for (const UMapNode* Node : ChildNodes)
	{
		MaxDepth = FMath::Max(MaxDepth, Node->GetGraphDepth());
	}

	return MaxDepth + 1;
}

void UMapNode::FixIntersections(const TArray<int32>& HeightLevels, const UMapNode* NodeWithIntersections)
{
	int32 HigherNeighbourIndex = -1;
	int32 LowerNeighbourIndex = ChildNodes.Num();
	// Height may change, so I need to iterate through all column to find neighbours of original node
	for (int32 i = 0; i < ChildNodes.Num(); ++i)
	{
		if (ChildNodes[i] == NodeWithIntersections)
		{
			HigherNeighbourIndex = i - 1;
			LowerNeighbourIndex = i + 1;
			break;
		}
	}

	if (HigherNeighbourIndex >= 0)
	{
		const int32 IntersectionsCount = CountIntersections(HeightLevels, ChildNodes[HigherNeighbourIndex]);
		ChildNodes[HigherNeighbourIndex]->UpdateHeightLevels(IntersectionsCount);
	}
	if (LowerNeighbourIndex < ChildNodes.Num())
	{
		const int32 IntersectionsCount = CountIntersections(HeightLevels, ChildNodes[LowerNeighbourIndex]);
		ChildNodes[LowerNeighbourIndex]->UpdateHeightLevels(-IntersectionsCount);
	}
}

int32 UMapNode::CountIntersections(const TArray<int32>& HeightLevels, const UMapNode* Node)
{
	int32 IntersectionsCount = 0;
	for (UMapNode* ChildN : Node->GetChildNodes())
	{
		for (int32 Height : HeightLevels)
		{
			if (ChildN->GetHeightLevel() == Height)
			{
				++IntersectionsCount;
				break;
			}
		}
	}

	return IntersectionsCount;
}

void UMapNode::UpdateHeightLevels(int32 HeightIncrement)
{
	HeightLevel += HeightIncrement;

	for (UMapNode* CNode : ChildNodes)
	{
		CNode->UpdateHeightLevels(HeightIncrement);
	}
}

void UMapNode::GenerateChildrenNodes(const int32 Num, const int32 MaxFacilitiesNum, const int32 NewDepth)
{
	for (int i = 0; i < Num; ++i)
	{
		ChildNodes.Add(GenerateNode(
			FMath::RandRange(1, MaxFacilitiesNum)));
		ChildNodes.Last()->SetDepth(NewDepth);
		ChildNodes.Last()->SetParentNode(this);
	}
}

UMapNode* UMapNode::GenerateNode(const int32 FacilitiesNum)
{
	UMapNode* Node = NewObject<UMapNode>(this);
	if (!Node)
		return nullptr;

	for (int i = 0; i < FacilitiesNum; ++i)
	{
		EFacilityType NewFacility = static_cast<EFacilityType>(
			FMath::RandHelper(EFacilityType::FACILITY_NR_ITEMS));
		Node->Facilities.Add(NewFacility);
	}

	return Node;
}
