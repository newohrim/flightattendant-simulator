// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInfo.generated.h"

class AFABaseCharacter;

UCLASS(BlueprintType, EditInlineNew)
class FLIGHTATTENDANTSIM_API UCharacterInfo : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TAssetSubclassOf<AFABaseCharacter> CharacterToSpawn;
	UPROPERTY(EditDefaultsOnly)
	FName CharacterName;
	UPROPERTY(EditDefaultsOnly)
	FText CharacterDisplayName;
	// Dialogs info (or i already set them in CharacterToSpawn?)
	// ............ 
};
