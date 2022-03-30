// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpacePlane/Seat.h"
#include "PassengerSeat.generated.h"

UCLASS()
class FLIGHTATTENDANTSIM_API APassengerSeat : public ASeat
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SeatTitle;
};
