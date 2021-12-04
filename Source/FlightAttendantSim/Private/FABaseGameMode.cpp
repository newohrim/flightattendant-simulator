// Fill out your copyright notice in the Description page of Project Settings.


#include "FABaseGameMode.h"
#include "DocsInfoStruct.h"
#include "TicketInfoStruct.h"

AFABaseGameMode::AFABaseGameMode() 
{
}

FString AFABaseGameMode::BuildDepartureTimeString(int32 Hours, int32 Minutes)
{
	return FString::FromInt(Hours)
		.AppendChar(':')
		.Append(Minutes > 9 ? FString::FromInt(Minutes) : FString("0").Append(FString::FromInt(Minutes)));
}

FString AFABaseGameMode::BuildDepartureDateString(int32 Day, int32 Month, int32 Year)
{
	return FString::FromInt(Day)
		.AppendChar('.')
		.Append(Month > 9 ? FString::FromInt(Month) : FString("0").Append(FString::FromInt(Month)))
		.AppendChar('.')
		.Append(FString::FromInt(Year));
}

UDocsInfoStruct* AFABaseGameMode::GetRandomDoc() const
{
	if (LastNames.Num() == 0 || FirstNames.Num() == 0 || Nationalities.Num() == 0)
		return NULL;
	const int32 PassengerId = FMath::RandRange(MinPassengerIdBound, MaxPassengerIdBound);
	const FString LastName = LastNames[FMath::RandRange(0, LastNames.Num() - 1)];
	const FString FirstName = FirstNames[FMath::RandRange(0, FirstNames.Num() - 1)];
	const FString Nationality = Nationalities[FMath::RandRange(0, Nationalities.Num() - 1)];
	return UDocsInfoStruct::CreateInstance(
		PassengerId,
		LastName,
		FirstName,
		Nationality,
		UDocsInfoStruct::GenerateRandomDate(GenerateDateYearFrom, GenerateDateYearTill),
		//UDocsInfoStruct::GenerateRandomDate(FlightDepartureDateYear - 6, FlightDepartureDateYear)
		UDocsInfoStruct::AddDaysToDate(FlightDepartureDate, FMath::RandRange(1, 720))
	);
}

UTicketInfoStruct* AFABaseGameMode::GetRandomTicket(const FString& SeatTitle) const
{
	if (LastNames.Num() == 0 || FirstNames.Num() == 0) 
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to generate random ticket. No info in LastNames, FirstNames or SeatTitles."));
		return NULL;
	}
	const int32 PassengerId = FMath::RandRange(MinPassengerIdBound, MaxPassengerIdBound);
	const FString LastName = LastNames[FMath::RandRange(0, LastNames.Num() - 1)];
	const FString FirstName = FirstNames[FMath::RandRange(0, FirstNames.Num() - 1)];
	return UTicketInfoStruct::CreateInstance(
		PassengerId,
		LastName,
		FirstName,
		//UDocsInfoStruct::GenerateRandomDate(GenerateDateYearFrom, GenerateDateYearTill),
		FlightDepartureDate,
		FlightDepartureTime,
		SeatTitle
	);
}

void AFABaseGameMode::InitDocuments()
{
	FlightDepartureTime = BuildDepartureTimeString(FlightDepartureTimeHours, FlightDepartureTimeMinutes);
	FlightDepartureDate = BuildDepartureDateString(FlightDepartureDateDay, FlightDepartureDateMonth, FlightDepartureDateYear);
	DocsContainer = UDocsInfoStruct::LoadUsingFileHelper(TCHAR_TO_UTF8(*PathToPassengersDocs));
	LastNames = UDocsInfoStruct::LoadUsingFileHelperStrings(TCHAR_TO_UTF8(*PathToLastNames));
	FirstNames = UDocsInfoStruct::LoadUsingFileHelperStrings(TCHAR_TO_UTF8(*PathToFirstNames));
	Nationalities = UDocsInfoStruct::LoadUsingFileHelperStrings(TCHAR_TO_UTF8(*PathToNationalities));
	TicketsContainer = GenerateTickets(SeatsCount);
	PassportsContainer = GeneratePassports(TicketsContainer);
}

TArray<UTicketInfoStruct*> AFABaseGameMode::GenerateTickets(int32 Count) const
{
	UTicketInfoStruct** RawArray = new UTicketInfoStruct*[Count];
	for (int i = 0; i < Count; ++i)
		RawArray[i] = GetRandomTicket(SeatTitles[i]);
	return TArray<UTicketInfoStruct*>(RawArray, Count);
}

TArray<UDocsInfoStruct*> AFABaseGameMode::GeneratePassports(
	const TArray<UTicketInfoStruct*>& TicketsList) const
{
	int32 Count = TicketsList.Num();
	UDocsInfoStruct** Passports = new UDocsInfoStruct*[Count];
	for (int i = 0; i < Count; ++i) 
	{
		Passports[i] = UDocsInfoStruct::CreateInstance(
			TicketsList[i]->GetPassengerId(),
			TicketsList[i]->GetLastName(),
			TicketsList[i]->GetFirstName(),
			GetRandomNationality(),
			UDocsInfoStruct::GenerateRandomDate(GenerateDateYearFrom, GenerateDateYearTill),
			//UDocsInfoStruct::GenerateRandomDate(GenerateDateYearFrom, GenerateDateYearTill)
			UDocsInfoStruct::AddDaysToDate(FlightDepartureDate, FMath::RandRange(1, 720))
			);
	}
	return TArray<UDocsInfoStruct*>(Passports, Count);
}

void AFABaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	InitDocuments();
	UE_LOG(LogTemp, Display, TEXT("Docs inited"));
}
