// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/Actions/CharacterAction.h"
#include "ActionExecutorComponent.generated.h"

class AFABaseCharacter;

DECLARE_DELEGATE(FActionsComplete)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UActionExecutorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FActionsComplete ActionsComplete;
	
	UActionExecutorComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	void SetActions(const TArray<FCharacterAction*> ActionsToSet) { Actions = ActionsToSet; }
	UFUNCTION(BlueprintCallable)
	void ExecuteActions();

protected:
	int32 ActiveIndex = 0;
	TArray<FCharacterAction*> Actions;
	UPROPERTY()
	AFABaseCharacter* LinkedCharacter;

	void NextAction();
	void CompleteActions();
};
