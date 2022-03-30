// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Actions/CharacterMoveAction.h"
#include "Characters/FABaseCharacter.h"

void UCharacterMoveAction::TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform)
{
	auto Result = CharacterToPerform->MoveTo(TargetLocation);
	if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		if (!CompleteAction())
			FailAction();
	}
	else if (Result == EPathFollowingRequestResult::Failed)
	{
		FailAction();
	}
}

UCharacterMoveAction* UCharacterMoveAction::CreateCharacterMoveAction(const FVector Location, UObject* Outer)
{
	UCharacterMoveAction* MoveAction = NewObject<UCharacterMoveAction>(Outer);
	MoveAction->TargetLocation = Location;
	return MoveAction;
}
