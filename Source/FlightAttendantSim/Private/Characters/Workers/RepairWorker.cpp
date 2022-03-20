// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Workers/RepairWorker.h"

#include "FAGameMode.h"
#include "SpacePlane/SpacePlaneComponent.h"
#include "SpacePlane/SpacePlaneHealthComponent.h"

FText ARepairWorker::GetParticipantCustomText_Implementation(FName ValueName) const
{
	if (ValueName == "RepairCost")
	{
		return FText::AsNumber(CalculateRepairCost());
	}
	if (ValueName == "RepairSmallCost")
	{
		return FText::AsNumber(SmallRepairCost);
	}
	if (ValueName == "RepairTime" || ValueName == "RepairTimeSmall")
	{
		return FText::AsNumber(CalculateRepairTime());
	}

	return Super::GetParticipantCustomText_Implementation(ValueName);
}

void ARepairWorker::RepairSpacePlane(int32 RepairCost, float RepairTime)
{
	// TODO: Player pays for repairment here.

	UWorld* World = GetWorld();
	if (World)
	{
		USpacePlaneHealthComponent* HealthComponent =
			Cast<AFAGameMode>(World->GetAuthGameMode())->GetSpacePlane()->GetSpacePlaneHealth();
		FTimerDelegate AddHealthDel;
		AddHealthDel.BindUObject(
			HealthComponent,
			&USpacePlaneHealthComponent::AddHealth,
			HealthComponent->GetMaxHealth());
		GetWorldTimerManager().SetTimer(
			RepairmentTimerHandle,
			AddHealthDel,
			RepairTime,
			false);
	}
}

float ARepairWorker::GetRepairPart() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		const USpacePlaneHealthComponent* HealthComponent =
			Cast<AFAGameMode>(World->GetAuthGameMode())->GetSpacePlane()->GetSpacePlaneHealth();
		if (HealthComponent->GetMaxHealth() > 0.0f)
		{
			return 1.0f - HealthComponent->GetHealth() / HealthComponent->GetMaxHealth();
		}
	}

	return 0.0f;
}
