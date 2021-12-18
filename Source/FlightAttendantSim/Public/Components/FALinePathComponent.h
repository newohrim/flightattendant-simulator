// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FALinePathComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLIGHTATTENDANTSIM_API UFALinePathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFALinePathComponent();

	// Returns location on line path based on Progress [0; 1]
	UFUNCTION(BlueprintCallable)
	virtual FVector GetLocationOnLinePath(float Progress) const;
	UFUNCTION(BlueprintCallable)
	void UpdateLinePath();

	UFUNCTION(BlueprintCallable)
	float GetPathDistance() const { return PathDistance; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Waypoints set in world space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinePathInfo")
	TArray<FVector> Waypoints;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float PathDistance = 0.0f;
	TArray<float> SegmentsBounds;

	int32 GetSegmentIndex(float Progress) const;
};
