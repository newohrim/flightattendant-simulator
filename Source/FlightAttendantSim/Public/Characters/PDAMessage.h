// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterInfo.h"
#include "PDAMessage.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPDAMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterInfo Author;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message;
};
