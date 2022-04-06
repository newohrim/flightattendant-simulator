// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Passengers/DocsInfo.h"
#include "PassengerSpawnParams.generated.h"

class ULocationInfo;

/**
 * 
 */
USTRUCT()
struct FPassengerSpawnParams
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform PassengerTransform;

	UPROPERTY()
	TSoftObjectPtr<ULocationInfo> LocationHeadingTo;

	UPROPERTY()
	FDocsInfo DocumentsInfo;

	bool IsAssigned = false;
};
