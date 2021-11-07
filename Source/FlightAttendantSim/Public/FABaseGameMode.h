// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <string>
#include "FABaseGameMode.generated.h"

class UDocsInfoStruct;
class UTicketInfoStruct;

UCLASS()
class FLIGHTATTENDANTSIM_API AFABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFABaseGameMode();

	UFUNCTION(BlueprintCallable)
	UDocsInfoStruct* GetRandomDoc() const;
	UFUNCTION(BlueprintCallable)
	UTicketInfoStruct* GetRandomTicket(const FString& SeatTitle) const;
	UFUNCTION(BlueprintCallable)
	TArray<UTicketInfoStruct*> GetTicketsList() const { return TicketsContainer; }
	UFUNCTION(BlueprintCallable)
	TArray<UDocsInfoStruct*> GetPassportsList() const { return PassportsContainer; }
	UFUNCTION(BlueprintCallable)
	void InitDocuments();
	UFUNCTION(BlueprintCallable)
	TArray<UTicketInfoStruct*> GenerateTickets(int32 Count) const;
	UFUNCTION(BlueprintCallable)
	TArray<UDocsInfoStruct*> GeneratePassports(const TArray<UTicketInfoStruct*>& TicketsList) const;
	UFUNCTION(BlueprintCallable)
	FString GetRandomNationality() const { return Nationalities[FMath::RandRange(0, Nationalities.Num() - 1)]; };
	virtual void InitGame(const FString& MapName,
		const FString& Options,
		FString& ErrorMessage) override;

protected:
	const static int32 MinPassengerIdBound = 100000000;
	const static int32 MaxPassengerIdBound = 999999999;

	UPROPERTY()
	TArray<UDocsInfoStruct*> DocsContainer;
	UPROPERTY()
	TArray<UTicketInfoStruct*> TicketsContainer;
	UPROPERTY()
	TArray<UDocsInfoStruct*> PassportsContainer;
	UPROPERTY(EditDefaultsOnly, Category = "PassengersInfo")
	TArray<FString> SeatTitles;
	UPROPERTY()
	TArray<FString> LastNames;
	UPROPERTY()
	TArray<FString> FirstNames;
	UPROPERTY()
	TArray<FString> Nationalities;
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToPassengersDocs;
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToLastNames;
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToFirstNames;
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToNationalities;

	UPROPERTY(EditDefaultsOnly, Category = "PassengersInfo")
	int32 SeatsCount = 6;
	UPROPERTY(EditDefaultsOnly, Category = "PassengersInfo")
	int32 GenerateDateYearFrom;
	UPROPERTY(EditDefaultsOnly, Category = "PassengersInfo")
	int32 GenerateDateYearTill;
};
