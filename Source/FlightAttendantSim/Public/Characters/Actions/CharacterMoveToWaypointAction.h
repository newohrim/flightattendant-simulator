// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Actions/CharacterMoveAction.h"
#include "CharacterMoveToWaypointAction.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UCharacterMoveToWaypointAction : public UCharacterMoveAction
{
	GENERATED_BODY()

public:
	virtual void TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UCharacterMoveToWaypointAction* CreateCharacterMoveToWaypointAction(
		const FString& WaypointName,
		UObject* Outer);

protected:
	bool IsFound = false;
	
	FString Waypoint;
};
