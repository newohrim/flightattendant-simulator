// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAction.generated.h"

class AFABaseCharacter;

DECLARE_DELEGATE(FActionComplete);
DECLARE_DELEGATE(FActionFailed);

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterAction
{
	GENERATED_BODY()

public:
	FActionComplete ActionComplete;
	FActionFailed ActionFailed;

	FCharacterAction() {}
	virtual ~FCharacterAction() {}
	virtual void TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform) { }
	
protected:
	bool CompleteAction() const;
	bool FailAction() const;
};
