// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Workers/Events/RepairSpaceShipFull.h"

#include "Characters/Workers/RepairWorker.h"

void URepairSpaceShipFull::EnterEvent_Implementation(UDlgContext* Context, UObject* Participant)
{
	ARepairWorker* RepairWorker = Cast<ARepairWorker>(Participant);
	if (RepairWorker)
	{
		RepairWorker->RepairSpacePlaneFull();
	}
	else UE_LOG(LogTemp, Error, TEXT("Participant must be a repair worker."));
}
