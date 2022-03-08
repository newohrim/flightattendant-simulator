// Fill out your copyright notice in the Description page of Project Settings.


#include "CargoDelivery/ShowDeliveryOffersEvent.h"

#include "FAGameMode.h"

void UShowDeliveryOffersEvent::EnterEvent_Implementation(UDlgContext* Context, UObject* Participant)
{
	Cast<AFAGameMode>(GetWorld()->GetAuthGameMode())->ShowCargoPickMenu();
}
