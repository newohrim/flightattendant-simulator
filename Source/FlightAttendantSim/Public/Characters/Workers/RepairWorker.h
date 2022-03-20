// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FABaseCharacter.h"
#include "RepairWorker.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API ARepairWorker : public AFABaseCharacter
{
	GENERATED_BODY()

public:
	virtual FText GetParticipantCustomText_Implementation(FName ValueName) const override;

	int32 CalculateRepairCost() const { return FMath::CeilToInt(GetRepairPart() * FullRepairCost); }

	int32 CalculateRepairTime() const { return FMath::CeilToInt(GetRepairPart() * FullRepairTime); }

	void RepairSpacePlaneFull() { RepairSpacePlane(CalculateRepairCost(), CalculateRepairTime()); }
	
	void RepairSpacePlaneSmall() { RepairSpacePlane(SmallRepairCost, CalculateRepairTime()); }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Repairment")
	int32 FullRepairCost = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Repairment")
	int32 SmallRepairCost = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Repairment")
	float FullRepairTime = 180;

	FTimerHandle RepairmentTimerHandle;

	float GetRepairPart() const;

	void RepairSpacePlane(int32 RepairCost, float RepairTime);
};
