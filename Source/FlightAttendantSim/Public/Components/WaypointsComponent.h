// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaypointsComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UWaypointsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWaypointsComponent();

	UFUNCTION(BlueprintCallable)
	bool GetWaypointByName(const FString& Name, FTransform& Result) const
	{
		const AActor* WaypointActor = Waypoints[Name];
		if (WaypointActor)
		{
			Result = Waypoints[Name]->GetActorTransform();
			return true;
		}
		UE_LOG(LogTemp, Warning,
			TEXT("Unable to find a waypoint named %s."), *Name);
		return false;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, AActor*> Waypoints;
};
