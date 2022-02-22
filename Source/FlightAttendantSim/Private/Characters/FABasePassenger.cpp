// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FABasePassenger.h"

#include "PassengerSeat.h"
#include "WorldMap/LocationInfo.h"

FText AFABasePassenger::GetParticipantCustomText_Implementation(FName ValueName) const
{
	if (ValueName == "HeadingToLocationName")
	{
		if (LocationHeadingTo)
		{
			return LocationHeadingTo->LocationName;
		}
		else
		{
			UE_LOG(LogTemp, Error,
				TEXT("LocationHeadingTo was anassigned on character %s."),
				*CharacterInfo.CharacterDisplayName.ToString())
		}
	}
	
	UE_LOG(LogTemp, Error,
		TEXT("Unknown custom text name %s on %s participant"),
		*ValueName.ToString(),
		*GetParticipantName().ToString());
	return Super::GetParticipantCustomText_Implementation(ValueName);
}

void AFABasePassenger::ShowDocuments() const
{
	// TODO: Show documents
}

void AFABasePassenger::AssignPassengerSeat(APassengerSeat* PassengerSeat)
{
	// Share of reference shit code
	PassengerSeat->AssignCharacter(this);
	AssignedPassengerSeat = PassengerSeat;
}

void AFABasePassenger::DeassignPassengerSeat()
{
	if (AssignedPassengerSeat)
		AssignedPassengerSeat->DeassignCharacter();
	AssignedPassengerSeat = nullptr;
}
