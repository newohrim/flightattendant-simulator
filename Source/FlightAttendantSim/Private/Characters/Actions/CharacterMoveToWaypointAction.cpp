// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Actions/CharacterMoveToWaypointAction.h"

#include "FAGameMode.h"
#include "Components/WaypointsComponent.h"

void UCharacterMoveToWaypointAction::TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform)
{
	if (!IsFound)
	{
		const UWaypointsComponent* Waypoints = FAGAMEMODE->GetWaypointsComponent();
		FTransform FoundWaypoint;
		if (Waypoints->GetWaypointByName(Waypoint, FoundWaypoint))
		{
			TargetLocation = FoundWaypoint.GetLocation();
			IsFound = true;
		}
		else
		{
			FailAction();
		}
	}
	
	Super::TickAction(DeltaTime, CharacterToPerform);
}

UCharacterMoveToWaypointAction* UCharacterMoveToWaypointAction::CreateCharacterMoveToWaypointAction(
	const FString& WaypointName,
	UObject* Outer)
{
	UCharacterMoveToWaypointAction* MoveAction = NewObject<UCharacterMoveToWaypointAction>(Outer);
	MoveAction->Waypoint = WaypointName;
	return MoveAction;
}
