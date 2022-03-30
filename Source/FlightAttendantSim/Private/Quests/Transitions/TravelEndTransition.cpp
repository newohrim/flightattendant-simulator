// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/Transitions/TravelEndTransition.h"

#include "FAGameMode.h"
#include "Flight/FlightControlComponent.h"
#include "WorldMap/MapNode.h"

UTravelEndTransition* UTravelEndTransition::CreateTravelEndTransition(UMapNode* Destination, UObject* Outer)
{
	UWorld* World = Outer->GetWorld();
	if (World)
	{
		UTravelEndTransition* Transition = NewObject<UTravelEndTransition>(Outer);
		Transition->DestinationNode = Destination;
		AFAGameMode* GameMode = Cast<AFAGameMode>(World->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->GetFlightController()->
				PlayerArrived.AddDynamic(Transition, &UTravelEndTransition::PlayerArrivedHandle);
			return Transition;
		}

		UE_LOG(LogTemp, Error, TEXT("Wrong gamemode. GameMode type must be FAGameMode."));
	}

	return nullptr;
}

void UTravelEndTransition::PlayerArrivedHandle(UMapNode* Destination)
{
	if (Destination && DestinationNode.Get() == Destination)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AFAGameMode* GameMode = Cast<AFAGameMode>(World->GetAuthGameMode());
			if (GameMode)
			{
				GameMode->GetFlightController()->
					PlayerArrived.RemoveDynamic(this, &UTravelEndTransition::PlayerArrivedHandle);
			}
		}
		ExecuteTransition();
	}
}
