// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/MapNode.h"

int32 UMapNode::GetDepth() const
{
	if (ChildNodes.Num() == 0)
		return 1;

	int32 MaxDepth = 1;
	for (const UMapNode* Node : ChildNodes)
	{
		MaxDepth = FMath::Max(MaxDepth, Node->GetDepth());
	}

	return MaxDepth + 1;
}

void UMapNode::GenerateChildrenNodes(const int32 Num, const int32 MaxFacilitiesNum)
{
	for (int i = 0; i < Num; ++i)
	{
		ChildNodes.Add(GenerateNode(
			FMath::RandRange(1, MaxFacilitiesNum)));
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
