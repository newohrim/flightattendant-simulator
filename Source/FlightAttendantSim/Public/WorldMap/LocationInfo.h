// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Characters/CharacterInfo.h"
#include "LocationInfo.generated.h"

/**
 * TODO: Remake as class.
 */
UCLASS()
class FLIGHTATTENDANTSIM_API ULocationInfo : public UObject
{
	GENERATED_BODY()

public:

protected:
	// Characters currently situated on this location
	UPROPERTY()
	TArray<FCharacterInfo> ResidentsCharacters;
};
