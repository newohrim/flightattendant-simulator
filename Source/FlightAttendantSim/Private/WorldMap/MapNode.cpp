// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/MapNode.h"
#include "WorldMap/LocationInfo.h"
#include "Quests/Quest.h"

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

int32 UMapNode::GetGraphMaxAbsHeight() const
{
	if (ChildNodes.Num() == 0)
		return FMath::Abs(HeightLevel);
	
	if (ChildNodes.Num() == 1)
		return ChildNodes[0]->GetGraphMaxAbsHeight();

	return FMath::Max(
		ChildNodes[0]->GetGraphMaxAbsHeight(),
		ChildNodes.Last()->GetGraphMaxAbsHeight());
}

TArray<UMapNode*> UMapNode::GetAccessibleNodes() const
{
	TArray<UMapNode*> ResultArray = ChildNodes;
	if (ParentNode)
	{
		for (UMapNode* Neighbour : ParentNode->GetChildNodes())
		{
			if (Neighbour != this)
			{
				ResultArray.Add(Neighbour);
			}
		}
	}

	return ResultArray;
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

void UMapNode::GenerateChildrenNodes(const TArray<UQuest*>& QuestsToPlace, const int32 NewDepth)
{
	int32 QuestsPerLoc = FMath::RandRange(1, 2);
	for (UQuest* Quest : QuestsToPlace)
	{
		auto& Locations = Quest->GetLocationsToGenerate();
        if (Locations.Num() == 0)
        {
        	UE_LOG(LogTemp, Error,
        		TEXT("%s quest has zero locations linked to it self."), 
        		*(Quest->GetQuestName()));
        	return;
        }

        UMapNode* CurrentChild = this;
		for (int32 j = 0; j < Locations.Num(); ++j)
		{
			// Forming a straight line of quest narration
			// (not as flexible as I'd like, but ok)
			// TODO: Make special array of spaces between locations. This way it will be more flexible.
			if (CurrentChild == this && QuestsPerLoc > 0 && ChildNodes.Num() > 0)
			{
				CurrentChild = ChildNodes.Last();
				CurrentChild->LocationInfo->Merge(Locations[j]);
			}
			else
			{
				CurrentChild->ChildNodes.Add(GenerateNode(Locations[j]));
				CurrentChild->ChildNodes.Last()->SetParentNode(CurrentChild);
				CurrentChild = ChildNodes.Last();
				CurrentChild->SetDepth(NewDepth + j);
			}
		}
		// Unreadable code X_X
		// One step delay
		if (QuestsPerLoc-- <= 0)
			QuestsPerLoc += FMath::RandRange(1, 2);
		Quest->SetIsPlaced(true);
		if (ChildNodes.Num() >= MaxChildrenCount)
			break;
	}
}

UMapNode* UMapNode::GenerateNode(ULocationInfo* LocInfo)
{
	UMapNode* Node = NewObject<UMapNode>(this);
	if (!Node)
		return nullptr;

	// Share of references (shit code?)
	Node->LocationInfo = LocInfo;
	LocInfo->CorrespondingNode = Node;
	/*
	for (int i = 0; i < LocationInfo.Facilities.Num(); ++i)
	{
		EFacilityType NewFacility = static_cast<EFacilityType>(
			FMath::RandHelper(EFacilityType::FACILITY_NR_ITEMS));
		Node->LocationInfo.Facilities.Add(NewFacility);
	}*/

	return Node;
}
