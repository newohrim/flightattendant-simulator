﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FABaseCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SpacePlane/Seat.h"
#include "Characters/DialogueComponent.h"


// Sets default values
AFABaseCharacter::AFABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());
	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>("DialogueComponent");
}

void AFABaseCharacter::MoveTo(const FVector TargetLocation)
{
	IsMoving = true;
	CurrentMovingTarget = TargetLocation;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed = 0.0f;
	
	/*
	AAIController* CharacterController = Cast<AAIController>(Controller);
	if (CharacterController)
	{
		// TODO: Remember moving result and analyse to fix any moving issues.
		CharacterController->MoveToLocation(TargetLocation);
	}
	*/
}

void AFABaseCharacter::SitOnSeat(ASeat* Seat)
{
	IsSitting = true;
	CurrentSeat = Seat;
	const FVector StaticPos = Seat->GetMoveToLocation().GetLocation();
	SetActorLocationAndRotation(FVector(StaticPos.X, StaticPos.Y, GetActorLocation().Z),
		Seat->GetMoveToLocation().Rotator());
	// Character falling under the plane after the call below
	// SetActorTransform(Seat->GetMoveToLocation());
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

void AFABaseCharacter::TalkTo(UObject* PlayerInstance)
{
	DialogueComponent->StartDialogue({PlayerInstance, this});
}

// Called every frame
void AFABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMoving)
	{
		Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed =
			FMath::FInterpConstantTo(Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed,
				InitialWalkSpeed, DeltaTime, 100.0f);
		
		AAIController* CharacterController = Cast<AAIController>(Controller);
		if (CharacterController)
		{
			// TODO: Remember moving result and analyse to fix any moving issues.
			const auto Result = CharacterController->MoveToLocation(CurrentMovingTarget);
			if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
			{
				IsMoving = false;
				TargetReached.Broadcast();
			}
		}
	}
}

// Called to bind functionality to input
void AFABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

