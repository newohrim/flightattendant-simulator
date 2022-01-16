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

void UMapNode::UpdateHeightLevels(int32 HeightIncrement)
{
	HeightLevel += HeightIncrement;

	for (UMapNode* CNode : ChildNodes)
	{
		CNode->UpdateHeightLevels(HeightIncrement);
	}
}

void UMapNode::MakeGridLayout(int32& X)
{
	if (ChildNodes.Num() == 0)
	{
		SetHeightLevel(X++);
		return;
	}
	
	const int32 Mid = ChildNodes.Num() / 2;
	for (int i = 0; i < ChildNodes.Num(); ++i)
	{
		if (i == Mid)
		{
			if ((ChildNodes.Num() & 1) == 1)
				SetHeightLevel(X);
			else
				SetHeightLevel(X++);
		}
		ChildNodes[i]->MakeGridLayout(X);
		//ChildNodes[i]->SetHeightLevel(X++);
		//ChildNodes[i]->SetDepth(Y);
	}
}

void UMapNode::GetConnectedPairs(TArray<FVector2D>& NodePairs) const
{
	for (UMapNode* Child : ChildNodes)
	{
		Child->GetConnectedPairs(NodePairs);
		NodePairs.Add(
			{static_cast<float>(GetDepth()), static_cast<float>(GetHeightLevel())});
		NodePairs.Add(
			{static_cast<float>(Child->GetDepth()), static_cast<float>(Child->GetHeightLevel())});
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
