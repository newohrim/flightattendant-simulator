// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DlgEventCustom.h"
#include "UObject/Object.h"
#include "GiveQuestEvent.generated.h"

class UQuest;

/**
 * 
 */
UCLASS()
class FLIGHTATTENDANTSIM_API UGiveQuestEvent : public UDlgEventCustom
{
	GENERATED_BODY()

public:
	virtual void EnterEvent_Implementation(UDlgContext* Context, UObject* Participant) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuest> QuestToGive;
};
