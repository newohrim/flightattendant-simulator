// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DialogueComponent.h"

#include "DlgDialogue.h"
#include "DlgManager.h"
#include "Nodes/DlgNode_SpeechSequence.h"

DEFINE_LOG_CATEGORY_STATIC(LogDialogueComponent, All, All)

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDialogueComponent::StartDialogue(const TArray<UObject*>& Participants)
{
	if (!AssignedDialogue)
	{
		UE_LOG(LogDialogueComponent, Error, TEXT("%s wasn't assigned with the DialogueAsset."), *GetFullName());
		return;
	}
	DialogueContext = UDlgManager::StartDialogue(AssignedDialogue, Participants);
	if (!DialogueContext)
	{
		UE_LOG(LogDialogueComponent, Error, TEXT("%s failed to load its assigned dialog."), *GetFullName());
	}
}

bool UDialogueComponent::ChooseDialogueOption(const int32 OptionIndex)
{
	if (!DialogueContext)
		return false;
	if (!DialogueContext->ChooseOption(OptionIndex))
	{
		DialogueContext = nullptr;
		return false;
	}

	return true;
}

void UDialogueComponent::EnterDialog_Implementation()
{
	// CREATE WIDGET AND INITIALIZE
}

void UDialogueComponent::ExitDialog_Implementation()
{
	// REMOVE WIDGET FROM VIEWPORT
}

