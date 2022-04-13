// Fill out your copyright notice in the Description page of Project Settings.


#include "FASaveGame.h"

#include "FASaveGameArchive.h"
#include "PassengersManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WorldMap/MapGraph.h"
#include "WorldMap/MapNode.h"
#include "WorldMap/LocationInfo.h"
#include "SpacePlane/SpacePlaneComponent.h"
#include "SpacePlane/SpacePlaneHealthComponent.h"
#include "SpacePlane/CargoCellComponent.h"
#include "Characters/FABasePassenger.h"

UFASaveGame::UFASaveGame()
{
	SaveSlotName = TEXT("MainSaveSlot");
	UserIndex = 0;
}

void UFASaveGame::PostLoadInitialization()
{
	MapGraphTraversalRead();
	USpacePlaneComponent* SpacePlaneComponent = SpacePlaneData.SpacePlane.Get();
	if (SpacePlaneComponent)
	{
		DeserializeObject(SpacePlaneComponent, SpacePlaneData.SpacePlaneData);
		DeserializeObject(SpacePlaneComponent->GetSpacePlaneHealth(), SpacePlaneData.HealthComponentData);
		DeserializeObject(SpacePlaneComponent->GetCargoCell(), SpacePlaneData.CargoCellData);
	}
}

void UFASaveGame::SaveWorldMap(const UMapGraph* MapGraph)
{
	WorldMap.Empty();
	WorldMap.Add(MapGraphTraversalWrite(MapGraph->GetRootNode(), MapGraph->GetCurrentNode()));
}

void UFASaveGame::SaveSpacePlane(USpacePlaneComponent* SpacePlaneComponent)
{
	SpacePlaneData.SpacePlane = SpacePlaneComponent;
	SerializeObject(SpacePlaneComponent, SpacePlaneData.SpacePlaneData);
	SerializeObject(SpacePlaneComponent->GetSpacePlaneHealth(), SpacePlaneData.HealthComponentData);
	SerializeObject(SpacePlaneComponent->GetCargoCell(), SpacePlaneData.CargoCellData);
}

void UFASaveGame::SavePassengers(const UPassengersManagerComponent* PassengersManager)
{
	// Save passengers info
	SpacePlaneData.AssignedPassengers.Empty();
	auto& Passengers = PassengersManager->GetSpawnedPassengers();
	for (const AFABasePassenger* Passenger : Passengers)
	{
		SpacePlaneData.AssignedPassengers.Add(
			{
				{
					Passenger->GetActorTransform(),
					Passenger->GetLocationHeadingTo(),
					Passenger->GetDocsInfo(),
					Passenger->IsAssigned()
				},
				FindNode(Passenger->GetLocationHeadingTo()->CorrespondingNode)
			});
	}
}

int32 UFASaveGame::FindNode(const UMapNode* Node) const
{
	for (int32 i = 0; i < WorldMap.Num(); ++i)
	{
		if (WorldMap[i].LocationInfo->CorrespondingNode == Node)
		{
			return i;
		}
	}

	return -1;
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
