// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Actions/CharacterAction.h"
#include "Characters/FABaseCharacter.h"

bool FCharacterAction::CompleteAction() const
{
	return ActionComplete.ExecuteIfBound();
}

bool FCharacterAction::FailAction() const
{
	return ActionFailed.ExecuteIfBound();
}
