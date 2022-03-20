// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FABaseCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SpacePlane/Seat.h"
#include "Characters/DialogueComponent.h"
#include "Characters/ActionExecutorComponent.h"


// Sets default values
AFABaseCharacter::AFABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>("DialogueComponent");
	ActionExecutorComponent = CreateDefaultSubobject<UActionExecutorComponent>("ActionExecutorComponent");
}

EPathFollowingRequestResult::Type AFABaseCharacter::MoveTo(const FVector TargetLocation)
{
	//Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed =
	//	FMath::FInterpConstantTo(Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed,
	//		InitialWalkSpeed, DeltaTime, 100.0f);

	IsMoving = true;
	AAIController* CharacterController = Cast<AAIController>(Controller);
	if (CharacterController)
	{
		// TODO: Remember moving result and analyse to fix any moving issues.
		const auto Result = CharacterController->MoveToLocation(TargetLocation);
		if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			IsMoving = false;
			TargetReached.Broadcast();
		}
		return Result;
	}

	return EPathFollowingRequestResult::Failed;
}

void AFABaseCharacter::SitOnSeat(ASeat* Seat)
{
	IsSitting = true;
	CurrentSeat = Seat;
	const FVector StaticPos = Seat->GetMoveToLocation().GetLocation();
	// Prevent player from falling through the ground
	GetCharacterMovement()->GravityScale = 0.0f;
	SetActorLocationAndRotation(FVector(StaticPos.X, StaticPos.Y, GetActorLocation().Z),
		Seat->GetMoveToLocation().Rotator());
}

void AFABaseCharacter::StandFromSeat()
{
	IsSitting = false;
	CurrentSeat = nullptr;
	PlayAnimMontage(SitToStandAnim);
}

// Called when the game starts or when spawned
void AFABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//InitialWalkSpeed = Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed;
}

FName AFABaseCharacter::GetParticipantName_Implementation() const
{
	return CharacterInfo.CharacterName;
}

FText AFABaseCharacter::GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const
{
	return CharacterInfo.CharacterDisplayName;
}

FText AFABaseCharacter::GetParticipantCustomText_Implementation(FName ValueName) const
{
	UE_LOG(LogTemp, Error,
		TEXT("Unknown custom text name %s on %s participant"),
		*ValueName.ToString(),
		*Execute_GetParticipantName(this).ToString());
	return FText::FromString("");
}

void AFABaseCharacter::TalkTo(UObject* PlayerInstance)
{
	DialogueComponent->StartDialogue({PlayerInstance, this});
}

// Called every frame
void AFABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AFABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

