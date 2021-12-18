// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FALinePathComponent.h"

// Sets default values for this component's properties
UFALinePathComponent::UFALinePathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


FVector UFALinePathComponent::GetLocationOnLinePath(float Progress) const
{
	if (Waypoints.Num() == 0)
		return GetOwner()->GetActorLocation();
	if (Waypoints.Num() == 1)
		return Waypoints[0];

	int SegmentIndex = GetSegmentIndex(Progress);
	FVector SegmentDirection = Waypoints[SegmentIndex + 1] - Waypoints[SegmentIndex];
	float SubProgress = 0.0f;
	if (SegmentIndex > 0)
		SubProgress = SegmentsBounds[SegmentIndex - 1];
	float SegmentProgress = SegmentsBounds[SegmentIndex] - SubProgress;
	if (SegmentProgress < 0.01f)
		SegmentProgress = 1.0f;
	
	FVector TargetLocation = Waypoints[SegmentIndex]
		+ SegmentDirection * ((Progress - SubProgress) / SegmentProgress);

	return TargetLocation;
}

void UFALinePathComponent::UpdateLinePath()
{
	if (Waypoints.Num() < 2)
		return;

	float* SegmentSizes = new float[Waypoints.Num() - 1];
	PathDistance = 0;
	for (int i = 1; i < Waypoints.Num(); ++i) 
	{
		SegmentSizes[i - 1] = (Waypoints[i] - Waypoints[i - 1]).Size();
		PathDistance += SegmentSizes[i - 1];
	}

	float SubDistance = 0.0f;
	SegmentsBounds.SetNum(Waypoints.Num() - 1);
	for (int i = 0; i < Waypoints.Num() - 1; ++i) 
	{
		SubDistance += SegmentSizes[i];
		SegmentsBounds[i] = SubDistance / PathDistance;
	}
	
	delete[] SegmentSizes;
}

// Called when the game starts
void UFALinePathComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UpdateLinePath();
}


// Called every frame
void UFALinePathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UFALinePathComponent::GetSegmentIndex(float Progress) const
{
	for (int i = 0; i < SegmentsBounds.Num(); ++i) 
	{
		if (Progress < SegmentsBounds[i]) 
		{
			return i;
		}
	}

	return SegmentsBounds.Num() - 1;
}

