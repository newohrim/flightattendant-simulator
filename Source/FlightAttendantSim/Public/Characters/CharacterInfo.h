// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInfo.generated.h"

class AFABaseCharacter;

USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CharacterName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CharacterDisplayName;
	// Dialogs info (or i already set them in CharacterToSpawn?)
	// ............ 
};
