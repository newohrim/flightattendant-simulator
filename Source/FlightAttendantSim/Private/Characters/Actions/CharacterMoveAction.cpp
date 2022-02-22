// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Actions/CharacterMoveAction.h"
#include "Characters/FABaseCharacter.h"

FCharacterMoveAction::FCharacterMoveAction(const FVector MoveToLocation)
{
	TargetLocation = MoveToLocation;
}

void FCharacterMoveAction::TickAction(const float DeltaTime, AFABaseCharacter* CharacterToPerform)
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
