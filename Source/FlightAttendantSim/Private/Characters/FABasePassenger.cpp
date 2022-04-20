// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FABasePassenger.h"

#include "FAGameMode.h"
#include "PassengerSeat.h"
#include "Components/PassengersManagerComponent.h"
#include "SpacePlaneComponent.h"
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
	else if (ValueName == "FakeTerrorist")
	{
		UWorld* World = GetWorld();
		if (World)
		{
			auto& AssignedPassengers =
				FAGAMEMODE->GetSpacePlane()->GetAssignedPassengers();
			if (AssignedPassengers.Num() >= 2)
			{
				const AFABasePassenger* FakeTerrorist = AssignedPassengers[AssignedPassengers.Num() - 2];
				return FText::FromString(FakeTerrorist->GetDocsInfo().LastName +
					" " + FakeTerrorist->GetDocsInfo().FirstName);
			}
		}
	}
	
	return Super::GetParticipantCustomText_Implementation(ValueName);
}

FText AFABasePassenger::GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const
{
	return FText::FromString(FString::Printf(
		TEXT("%s %s"), *DocumentsInfo.LastName, *DocumentsInfo.FirstName));
}

void AFABasePassenger::ShowDocuments_Implementation() const
{
	// PLACEHOLDER
}

void AFABasePassenger::UnShowDocuments_Implementation(bool IsApproved) const
{
	// PLACEHOLDER
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

void AFABasePassenger::BeginPlay()
{
	Super::BeginPlay();

	// If passenger was placed on the scene in editor then DocumentsInfo is not initialized
	// So I proceed it here
	if (DocumentsInfo.PassengerId.Len() == 0)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			DocumentsInfo =
				Cast<AFAGameMode>(World->GetAuthGameMode())->GetPassengerManager()->CreateDocument();
		}
	}
}
