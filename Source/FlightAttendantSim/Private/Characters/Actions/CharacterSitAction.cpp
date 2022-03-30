// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Actions/CharacterSitAction.h"
#include "Characters/FABaseCharacter.h"

void UCharacterSitAction::TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform)
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

UCharacterSitAction* UCharacterSitAction::CreateCharacterSitAction(ASeat* Seat, UObject* Outer)
{
	UCharacterSitAction* SitAction = NewObject<UCharacterSitAction>(Outer);
	SitAction->SeatToSitOn = Seat;
	return SitAction;
}
