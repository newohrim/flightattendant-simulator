// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FAGameInstance.generated.h"


/**
 * May be used for storing different game data.
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UFAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool IsNewGame = true;
};
