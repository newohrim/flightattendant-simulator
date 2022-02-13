// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Seat.generated.h"

UCLASS()
class FLIGHTATTENDANTSIM_API ASeat : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASeat();

	UFUNCTION(BlueprintCallable)
	const FTransform& GetMoveToLocation() const { return MoveToLocation->GetComponentTransform(); }
	UFUNCTION(BlueprintCallable)
	const FTransform& GetSnapLocation() const { return SnapLocation->GetComponentTransform(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SeatMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* MoveToLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SnapLocation;
};
