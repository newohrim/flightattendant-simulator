// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DlgContext.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"

class UDlgDialogue;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDialogueComponent();

	UFUNCTION(BlueprintCallable)
	FText GetSpeakerName() const { return DialogueContext->GetActiveNodeParticipantDisplayName(); }
	UFUNCTION(BlueprintCallable)
	const FText& GetDialogueText() const { return DialogueContext->GetActiveNodeText(); }
	UFUNCTION(BlueprintCallable)
	const TArray<FDlgEdge>& GetDialogueOptions() const { return DialogueContext->GetOptionsArray(); }
	UFUNCTION(BlueprintCallable)
	const TArray<FDlgEdgeData>& GetAllDialogueOptions() const { return DialogueContext->GetAllOptionsArray(); }
	UFUNCTION(BlueprintCallable)
	virtual void StartDialogue(const TArray<UObject*>& Participants);
	UFUNCTION(BlueprintCallable)
	virtual bool ChooseDialogueOption(const int32 OptionIndex);

	UFUNCTION(BlueprintNativeEvent)
	void EnterDialog();
	UFUNCTION(BlueprintNativeEvent)
	void ExitDialog();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDlgDialogue* AssignedDialogue;
	UPROPERTY(BlueprintReadOnly)
	UDlgContext* DialogueContext;
};
