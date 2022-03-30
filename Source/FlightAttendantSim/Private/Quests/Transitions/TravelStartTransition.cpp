// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelStartTransition.h"

#include "FAGameMode.h"
#include "Flight/FlightControlComponent.h"
#include "WorldMap/MapNode.h"

UTravelStartTransition* UTravelStartTransition::CreateTravelStartTransition(UMapNode* Destination, UObject* Outer)
{
	UWorld* World = Outer->GetWorld();
	if (World)
	{
		UTravelStartTransition* Transition = NewObject<UTravelStartTransition>(Outer);
		Transition->DestinationNode = Destination;
		AFAGameMode* GameMode = Cast<AFAGameMode>(World->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->GetFlightController()->
				PlayerStartTravel.AddDynamic(Transition, &UTravelStartTransition::PlayerStartTravelHandle);
			return Transition;
		}

		UE_LOG(LogTemp, Error, TEXT("Wrong gamemode. GameMode type must be FAGameMode."));
	}

	return nullptr;
}

void UTravelStartTransition::PlayerStartTravelHandle(UMapNode* Destination)
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
					PlayerStartTravel.RemoveDynamic(this, &UTravelStartTransition::PlayerStartTravelHandle);
			}
		}
		ExecuteTransition();
	}
}
