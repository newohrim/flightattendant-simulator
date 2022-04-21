// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionExecutorComponent.h"
#include "Characters/FABaseCharacter.h"


// Sets default values for this component's properties
UActionExecutorComponent::UActionExecutorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UActionExecutorComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
	AActor* Owner = GetOwner();
	if (Owner->IsA(AFABaseCharacter::StaticClass()))
	{
		LinkedCharacter = Cast<AFABaseCharacter>(Owner);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("Owner of ActionExecutorComponent must be FABaseCharacter, but was %s."),
			*Owner->GetClass()->GetFullName());
	}
	check(LinkedCharacter);
}

// Called every frame
void UActionExecutorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsPaused && Actions.IsValidIndex(ActiveIndex))
		Actions[ActiveIndex]->TickAction(DeltaTime, LinkedCharacter);
}

void UActionExecutorComponent::ExecuteActions()
{
	if (Actions.Num() == 0)
	{
		UE_LOG(LogTemp, Error,
			TEXT("ExecuteActions was called, but no actions were set in ActionExecutorComponent."));
		return;
	}
	ActiveIndex = 0;
	Actions[ActiveIndex]->ActionComplete
			.BindUObject(this, &UActionExecutorComponent::NextAction);
	SetComponentTickEnabled(true);
}

void UActionExecutorComponent::PauseCurrentAction()
{
	SetComponentTickEnabled(false);
	IsPaused = true;
}

void UActionExecutorComponent::ResumeCurrentAction()
{
	if (Actions.Num() > 0)
		SetComponentTickEnabled(true);
	IsPaused = false;
}

void UActionExecutorComponent::NextAction()
{
	Actions[ActiveIndex]->ActionComplete.Unbind();
	++ActiveIndex;
	if (ActiveIndex < Actions.Num())
	{
		Actions[ActiveIndex]->ActionComplete
			.BindUObject(this, &UActionExecutorComponent::NextAction);
	}
	else
	{
		CompleteActions();
	}
}

void UActionExecutorComponent::CompleteActions()
{
	// May awake memory leak here?
	Actions.Empty();
	SetComponentTickEnabled(false);
	ActionsComplete.ExecuteIfBound();
}

