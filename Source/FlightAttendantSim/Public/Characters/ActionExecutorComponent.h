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

	UFUNCTION(BlueprintCallable)
	void SetActions(const TArray<UCharacterAction*>& ActionsToSet) { Actions = ActionsToSet; }
	
	UFUNCTION(BlueprintCallable)
	void ExecuteActions();

	UFUNCTION(BlueprintCallable)
	void PauseCurrentAction();

	UFUNCTION(BlueprintCallable)
	void ResumeCurrentAction();

protected:
	bool IsPaused = false;
	
	int32 ActiveIndex = 0;
	
	UPROPERTY()
	TArray<UCharacterAction*> Actions;
	
	UPROPERTY()
	AFABaseCharacter* LinkedCharacter;

	void NextAction();
	
	void CompleteActions();
};
