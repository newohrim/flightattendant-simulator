// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <string>
#include "FABaseGameMode.generated.h"

class UDocsInfoStruct;

UCLASS()
class FLIGHTATTENDANTSIM_API AFABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFABaseGameMode();

	UFUNCTION(BlueprintCallable)
	UDocsInfoStruct* GetRandomDoc() const;
	UFUNCTION(BlueprintCallable)
	void InitDocuments();
	virtual void InitGame(const FString& MapName,
		const FString& Options,
		FString& ErrorMessage) override;

protected:
	const static int32 MinPassengerIdBound = 100000000;
	const static int32 MaxPassengerIdBound = 999999999;

	//UPROPERTY()
	TArray<UDocsInfoStruct*> DocsContainer;
	//UPROPERTY()
	TArray<std::string> LastNames;
	//UPROPERTY()
	TArray<std::string> FirstNames;
	//UPROPERTY()
	TArray<std::string> Nationalities;
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToPassengersDocs;
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToLastNames;
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToFirstNames;
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToNationalities;

	UPROPERTY(EditDefaultsOnly, Category = "PassengersInfo")
	int32 GenerateDateYearFrom;
	UPROPERTY(EditDefaultsOnly, Category = "PassengersInfo")
	int32 GenerateDateYearTill;
};
