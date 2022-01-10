// Fill out your copyright notice in the Description page of Project Settings.


#include "FAGameInstance.h"

void UFAGameInstance::Init()
{
	Super::Init();

	// Just for debug purpose
	WorldMap = NewObject<UMapGraph>(this);
	if (WorldMap)
	{
		WorldMap->GenerateMap(WorldMapMaxDepth);
	}
}
