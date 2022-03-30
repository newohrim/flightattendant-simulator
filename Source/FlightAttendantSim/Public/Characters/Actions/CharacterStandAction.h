// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Actions/CharacterAction.h"
#include "CharacterStandAction.generated.h"

class ASeat;

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UCharacterStandAction : public UCharacterAction
{
	GENERATED_BODY()

public:
	virtual void TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UCharacterStandAction* CreateCharacterStandAction(UObject* Outer);
};
