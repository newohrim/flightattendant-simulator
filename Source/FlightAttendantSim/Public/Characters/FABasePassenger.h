// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Passengers/DocsInfo.h"
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowDocuments() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnShowDocuments(bool IsApproved) const;

	void AssignPassengerSeat(APassengerSeat* PassengerSeat);
	void DeassignPassengerSeat();
	APassengerSeat* GetAssignedPassengerSeat() const { return AssignedPassengerSeat; }
	void SetDocsInfo(const FDocsInfo DocsInfo) { DocumentsInfo = DocsInfo; }
	void SetDestinationLocation(ULocationInfo* Destination) { LocationHeadingTo = Destination; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	ULocationInfo* LocationHeadingTo;
	UPROPERTY(BlueprintReadOnly)
	APassengerSeat* AssignedPassengerSeat;
	UPROPERTY(BlueprintReadOnly)
	FDocsInfo DocumentsInfo;

	virtual void BeginPlay() override;
};
