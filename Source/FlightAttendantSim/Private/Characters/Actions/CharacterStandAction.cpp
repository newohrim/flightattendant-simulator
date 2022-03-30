// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Actions/CharacterStandAction.h"

#include "Characters/FABaseCharacter.h"
#include "SpacePlane/Seat.h"

void UCharacterStandAction::TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform)
{
	// May fail, but who bother?
	CharacterToPerform->StandFromSeat();
	if (!ActionComplete.ExecuteIfBound())
		ActionFailed.ExecuteIfBound();
}

UCharacterStandAction* UCharacterStandAction::CreateCharacterStandAction(UObject* Outer)
{
	return NewObject<UCharacterStandAction>(Outer);
}
