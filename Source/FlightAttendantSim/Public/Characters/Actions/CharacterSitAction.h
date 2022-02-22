// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAction.h"
#include "CharacterSitAction.generated.h"

class ASeat;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterSitAction : public FCharacterAction
{
	GENERATED_BODY()

public:
	FCharacterSitAction() = default;
	FCharacterSitAction(ASeat* Seat);
	virtual void TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform) override;

protected:
	UPROPERTY()
	ASeat* SeatToSitOn;
};
