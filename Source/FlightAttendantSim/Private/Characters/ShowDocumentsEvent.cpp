// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowDocumentsEvent.h"
#include "Characters/FABasePassenger.h"

void UShowDocumentsEvent::EnterEvent_Implementation(UDlgContext* Context, UObject* Participant)
{
	if (Participant->IsA(AFABasePassenger::StaticClass()))
	{
		const AFABasePassenger* Passenger = Cast<AFABasePassenger>(Participant);
		Passenger->ShowDocuments();
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("Participant must be AFABasePassenger, but was %s."),
			*Participant->GetFullName());
	}
}
