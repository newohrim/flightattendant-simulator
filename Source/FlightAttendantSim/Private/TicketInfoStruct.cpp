// Fill out your copyright notice in the Description page of Project Settings.

#include "TicketInfoStruct.h"

UTicketInfoStruct* UTicketInfoStruct::CreateInstance(
	const int32 PassengerId,
	const FString& LastName,
	const FString& FirstName,
	const FString& DepartureDate,
	const FString& DepartureTime,
	const FString& Seat) 
{
	UTicketInfoStruct* CreatedInstance = NewObject<UTicketInfoStruct>(UTicketInfoStruct::StaticClass());
	CreatedInstance->PassengerId = PassengerId;
	CreatedInstance->LastName = LastName;
	CreatedInstance->FirstName = FirstName;
	CreatedInstance->DepartureDate = DepartureDate;
	CreatedInstance->DepartureTime = DepartureTime;
	CreatedInstance->Seat = Seat;
	return CreatedInstance;
}

UTicketInfoStruct* UTicketInfoStruct::CopyInstance(const UTicketInfoStruct* OriginalInstance)
{
	return UTicketInfoStruct::CreateInstance(
		OriginalInstance->GetPassengerId(),
		OriginalInstance->GetLastName(),
		OriginalInstance->GetFirstName(),
		OriginalInstance->GetDepartureDate(),
		OriginalInstance->GetDepartureTime(),
		OriginalInstance->GetSeatTitle());
}
