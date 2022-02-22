// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LetInPassengerEvent.h"
#include "Characters/FABasePassenger.h"
#include "FAGameMode.h"

void ULetInPassengerEvent::EnterEvent_Implementation(UDlgContext* Context, UObject* Participant)
{
	if (Participant->IsA(AFABasePassenger::StaticClass()))
    {
    	AFABasePassenger* Passenger = Cast<AFABasePassenger>(Participant);
    	AFAGameMode* GameMode = Cast<AFAGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->LetPassengerInPlane(Passenger);
    }
    else
    {
    	UE_LOG(LogTemp, Error,
    		TEXT("Participant must be AFABasePassenger, but was %s."),
    		*Participant->GetFullName());
    }
}
