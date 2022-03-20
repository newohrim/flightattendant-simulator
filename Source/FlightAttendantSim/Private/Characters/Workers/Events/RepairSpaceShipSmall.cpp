// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Workers/Events/RepairSpaceShipSmall.h"

#include "Characters/Workers/RepairWorker.h"

void URepairSpaceShipSmall::EnterEvent_Implementation(UDlgContext* Context, UObject* Participant)
{
	ARepairWorker* RepairWorker = Cast<ARepairWorker>(Participant);
	if (RepairWorker)
	{
		RepairWorker->RepairSpacePlaneSmall();
	}
	else UE_LOG(LogTemp, Error, TEXT("Participant must be a repair worker."));
}
