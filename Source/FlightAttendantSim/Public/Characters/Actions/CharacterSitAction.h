// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Actions/CharacterAction.h"
#include "CharacterSitAction.generated.h"

class ASeat;

/**
 * 
 */
UCLASS(BlueprintType)
class UCharacterSitAction : public UCharacterAction
{
	GENERATED_BODY()

public:
	virtual void TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UCharacterSitAction* CreateCharacterSitAction(ASeat* Seat, UObject* Outer);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASeat* SeatToSitOn;
};
