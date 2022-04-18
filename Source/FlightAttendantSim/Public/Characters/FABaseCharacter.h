// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ObjectReceiver.h"
#include "Characters/CharacterInfo.h"
#include "../../../../Plugins/DlgSystem-v15/Source/DlgSystem/Public/DlgDialogueParticipant.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "FABaseCharacter.generated.h"

class ASeat;
class UDialogueComponent;
class UActionExecutorComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetReached);

UCLASS()
class FLIGHTATTENDANTSIM_API AFABaseCharacter : public ACharacter, public IDlgDialogueParticipant, public IObjectReceiver
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFABaseCharacter();

	UFUNCTION(BlueprintCallable)
	EPathFollowingRequestResult::Type MoveTo(const FVector TargetLocation);
	UFUNCTION(BlueprintCallable)
	void SitOnSeat(ASeat* Seat);
	UFUNCTION(BlueprintCallable)
	void StandFromSeat();
	UFUNCTION(BlueprintCallable)
	ASeat* GetCurrentSeat() const { return CurrentSeat; }
	UFUNCTION(BlueprintCallable)
	UActionExecutorComponent* GetActionExecutorComponent() const { return ActionExecutorComponent; }

	UPROPERTY(BlueprintAssignable)
	FTargetReached TargetReached;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCharacterInfo CharacterInfo;

	virtual FName GetParticipantName_Implementation() const override;
	virtual FText GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const override;
	virtual FText GetParticipantCustomText_Implementation(FName ValueName) const override;
	UFUNCTION(BlueprintCallable)
	virtual void TalkTo(UObject* PlayerInstance);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDialogueComponent* DialogueComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UActionExecutorComponent* ActionExecutorComponent;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* SitToStandAnim;
	UPROPERTY(BlueprintReadWrite)
	bool IsSitting = false;
	UPROPERTY(BlueprintReadOnly)
	ASeat* CurrentSeat = nullptr;

private:
	bool IsMoving = false;
	FVector CurrentMovingTarget;
	float InitialWalkSpeed = 100.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
