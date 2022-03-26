// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/MapGraph.h"

#include "LocationInfo.h"
#include "WorldMap/TreeLayoutDrawing/TreeLayoutDrawer.h"
#include "WorldMap/TreeLayoutDrawing/TreeLayeredLayoutDrawer.h"
#include "WorldMap/MapNode.h"

void UMapGraph::BeginDestroy()
{
	UObject::BeginDestroy();

	delete TreeDrawer;
}

void UMapGraph::GenerateMap(int32 Depth, const TArray<UQuest*>& QuestsToPlace)
{
	// Manual cleanup is in BeginDestroy()
	TreeDrawer = new FTreeLayeredLayoutDrawer();
	
	this->MaxDepth = Depth;

	RootNode = NewObject<UMapNode>(this);
	RootNode->SetHeightLevel(0);
	ULocationInfo* RootLocation = NewObject<ULocationInfo>(this);
	RootLocation->CorrespondingNode = RootNode;
	RootLocation->LocationName = FText::FromString("RootStation");
	RootNode->SetLocationInfo(RootLocation);
	ExpandNode(RootNode, QuestsToPlace);
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

void UMapGraph::ExpandNode(UMapNode* Node, const TArray<UQuest*>& QuestsToPlace)
{
	if (Node->GetChildNodes().Num() > 0)
		return;

	Node->GenerateChildrenNodes(QuestsToPlace, Node->GetDepth() + 1);
	//FixIntersections(Node);
	//int32 X = 0;
	//RootNode->MakeGridLayout(X);
	// Max height is in X after MakeGridLayout call, so I make a half offset
	//CurrentMaxHeight = X / 2;
	//CurrentMaxDepth = GetGraphDepth();
	//RootNode->UpdateHeightLevels(-CurrentMaxHeight - (RootNode->GetHeightLevel() - CurrentMaxHeight));
	if (TreeDrawer)
	{
		TreeDrawer->MakeGridLayout(RootNode);
	}
	NodesPairs.Empty();
	RootNode->GetConnectedPairs(NodesPairs);
}
