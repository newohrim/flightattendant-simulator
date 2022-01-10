// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WorldMap/MapGraph.h"
#include "FAGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UFAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
protected:
	constexpr static int32 WorldMapMaxDepth = 10;
	
	UPROPERTY(BlueprintReadOnly)
	UMapGraph* WorldMap;
};
