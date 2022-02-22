// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Actions/CharacterSitAction.h"
#include "Characters/FABaseCharacter.h"

FCharacterSitAction::FCharacterSitAction(ASeat* Seat)
{
	SeatToSitOn = Seat;
}

void FCharacterSitAction::TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform)
{
	if (!SeatToSitOn)
	{
		SeatToSitOn = CharacterToPerform->GetCurrentSeat();
		if (!SeatToSitOn)
		{
			ActionFailed.ExecuteIfBound();
		}
	}
	CharacterToPerform->SitOnSeat(SeatToSitOn);
	if (!ActionComplete.ExecuteIfBound())
		ActionFailed.ExecuteIfBound();
}
