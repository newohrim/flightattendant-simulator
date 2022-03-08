// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TransitionEvent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, Abstract)
class FLIGHTATTENDANTSIM_API UTransitionEvent : public UObject
{
	GENERATED_BODY()

public:
	virtual void Execute() {}
};
