// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DlgEventCustom.h"
#include "RepairSpaceShipFull.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API URepairSpaceShipFull : public UDlgEventCustom
{
	GENERATED_BODY()

public:
	virtual void EnterEvent_Implementation(UDlgContext* Context, UObject* Participant) override;
};
