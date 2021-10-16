// Fill out your copyright notice in the Description page of Project Settings.


#include "FABaseGameMode.h"
#include "DocsInfoStruct.h"

AFABaseGameMode::AFABaseGameMode() 
{
}

UDocsInfoStruct* AFABaseGameMode::GetRandomDoc() const
{
	if (LastNames.Num() == 0 || FirstNames.Num() == 0 || Nationalities.Num() == 0)
		return NULL;
	const int32 PassengerId = FMath::RandRange(MinPassengerIdBound, MaxPassengerIdBound);
	const std::string LastName = LastNames[FMath::RandRange(0, LastNames.Num() - 1)];
	const std::string FirstName = FirstNames[FMath::RandRange(0, FirstNames.Num() - 1)];
	const std::string Nationality = Nationalities[FMath::RandRange(0, Nationalities.Num() - 1)];
	return UDocsInfoStruct::CreateInstance(
		PassengerId,
		LastName,
		FirstName,
		Nationality,
		UDocsInfoStruct::GenerateRandomDate(GenerateDateYearFrom, GenerateDateYearTill),
		UDocsInfoStruct::GenerateRandomDate(GenerateDateYearFrom, GenerateDateYearTill)
		);
}

void AFABaseGameMode::InitDocuments()
{
	//DocsContainer = UDocsInfoStruct::LoadUsingFileHelper(TCHAR_TO_UTF8(*PathToPassengersDocs));
	LastNames = UDocsInfoStruct::LoadUsingFileHelperStrings(TCHAR_TO_UTF8(*PathToLastNames));
	FirstNames = UDocsInfoStruct::LoadUsingFileHelperStrings(TCHAR_TO_UTF8(*PathToFirstNames));
	Nationalities = UDocsInfoStruct::LoadUsingFileHelperStrings(TCHAR_TO_UTF8(*PathToNationalities));
}

void AFABaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Display, TEXT("Docs inited"));
	//DocsContainer = UDocsInfoStruct::Load(TCHAR_TO_UTF8(*PathToPassengersDocs));
	InitDocuments();
}
