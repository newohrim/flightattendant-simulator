// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FABaseCharacter.h"
#include "FABasePassenger.generated.h"

class ULocationInfo;
class APassengerSeat;

UCLASS()
class FLIGHTATTENDANTSIM_API AFABasePassenger : public AFABaseCharacter
{
	GENERATED_BODY()

public:
	virtual FText GetParticipantCustomText_Implementation(FName ValueName) const override;

	void ShowDocuments() const;

	void AssignPassengerSeat(APassengerSeat* PassengerSeat);
	void DeassignPassengerSeat();
	APassengerSeat* GetAssignedPassengerSeat() const { return AssignedPassengerSeat; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	ULocationInfo* LocationHeadingTo;
	UPROPERTY(BlueprintReadOnly)
	APassengerSeat* AssignedPassengerSeat;
};
