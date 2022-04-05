// Fill out your copyright notice in the Description page of Project Settings.


#include "FASaveGame.h"

#include "FASaveGameArchive.h"
#include "Kismet/GameplayStatics.h"
#include "WorldMap/MapGraph.h"
#include "WorldMap/MapNode.h"
#include "WorldMap/LocationInfo.h"

UFASaveGame::UFASaveGame()
{
	SaveSlotName = TEXT("MainSaveSlot");
	UserIndex = 0;
}

void UFASaveGame::PostLoadInitialization()
{
	MapGraphTraversalRead();
}

void UFASaveGame::SaveWorldMap(const UMapGraph* MapGraph)
{
	WorldMap.Empty();
	WorldMap.Add(MapGraphTraversalWrite(MapGraph->GetRootNode(), MapGraph->GetCurrentNode()));
}

FMapNodeData UFASaveGame::MapGraphTraversalWrite(const UMapNode* Node, const UMapNode* CurrentNode)
{
	FMapNodeData NodeData;
	NodeData.LocationInfo = Node->GetLocationInfo();
	if (Node == CurrentNode)
		NodeData.IsCurrent = true;
	for (const UMapNode* Child : Node->GetChildNodes())
	{
		WorldMap.Add(MapGraphTraversalWrite(Child, CurrentNode));
		NodeData.Children.Add(WorldMap.Num() - 1);
	}

	SerializeObject(NodeData.LocationInfo.Get(), NodeData.LocationData);
	return NodeData;
}

void UFASaveGame::MapGraphTraversalRead()
{
	for (FMapNodeData& Node : WorldMap)
	{
		Node.LocationInfo = NewObject<ULocationInfo>(this);
		DeserializeObject(Node.LocationInfo.Get(), Node.LocationData);
	}
}

void UFASaveGame::SerializeObject(UObject* Object, TArray<uint8>& OutputData)
{
	if (!Object)
		return;

	FMemoryWriter MemoryWriter(OutputData, true);
	FFASaveGameArchive MyArchive(MemoryWriter);
	Object->Serialize(MyArchive);
}

void UFASaveGame::DeserializeObject(UObject* Object, TArray<uint8>& InputData)
{
	if (!Object)
		return;

	FMemoryReader MemoryReader(InputData, true);
	FFASaveGameArchive MyArchive(MemoryReader);
	Object->Serialize(MyArchive);
}
