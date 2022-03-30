// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Actions/CharacterAction.h"
#include "CharacterMoveAction.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UCharacterMoveAction : public UCharacterAction
{
	GENERATED_BODY()

public:
	virtual void TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UCharacterMoveAction* CreateCharacterMoveAction(FVector Location, UObject* Outer);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;
};
