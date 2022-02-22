// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAction.h"
#include "CharacterMoveAction.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterMoveAction : public FCharacterAction
{
	GENERATED_BODY()

public:
	FCharacterMoveAction() = default;
	FCharacterMoveAction(const FVector MoveToLocation);
	virtual void TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform) override;

protected:
	FVector TargetLocation;
};
