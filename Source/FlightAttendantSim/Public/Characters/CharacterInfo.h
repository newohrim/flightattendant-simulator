// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterInfo.generated.h"

USTRUCT()
struct FLIGHTATTENDANTSIM_API FCharacterInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	TAssetSubclassOf<AActor> CharacterToSpawn;
	UPROPERTY(EditDefaultsOnly)
	FName CharacterName;
	UPROPERTY(EditDefaultsOnly)
	FText CharacterDisplayName;
	// Dialogs info
	// ............
};
