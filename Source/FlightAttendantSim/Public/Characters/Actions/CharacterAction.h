// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterAction.generated.h"

class AFABaseCharacter;

DECLARE_DELEGATE(FActionComplete);
DECLARE_DELEGATE(FActionFailed);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew)
class FLIGHTATTENDANTSIM_API UCharacterAction : public UObject
{
	GENERATED_BODY()

public:
	FActionComplete ActionComplete;
	FActionFailed ActionFailed;

	virtual void TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform) { }
	
protected:
	bool CompleteAction() const;
	bool FailAction() const;
};
