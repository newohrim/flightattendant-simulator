// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TicketInfoStruct.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FLIGHTATTENDANTSIM_API UTicketInfoStruct : public UObject
{
	GENERATED_BODY()

public:
	static UTicketInfoStruct* CreateInstance(
		const int32 PassengerId,
		const FString& LastName,
		const FString& FirstName,
		const FString& DepartureDate,
		const FString& DepartureTime,
		const FString& Seat);
	UFUNCTION(BlueprintCallable)
	static UTicketInfoStruct* CopyInstance(const UTicketInfoStruct* OriginalInstance);
	//static FString GenerateSeatNumber(int32 Min, int);
	UFUNCTION(BlueprintCallable)
	int32 GetPassengerId() const { return PassengerId; }
	UFUNCTION(BlueprintCallable)
	FString GetLastName() const { return LastName; }
	UFUNCTION(BlueprintCallable)
	FString GetFirstName() const { return FirstName; }
	UFUNCTION(BlueprintCallable)
	FString GetDepartureDate() const { return DepartureDate; }
	UFUNCTION(BlueprintCallable)
	FString GetDepartureTime() const { return DepartureTime; }
	UFUNCTION(BlueprintCallable)
	FString GetSeatTitle() const { return Seat; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	int32 PassengerId = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString LastName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString FirstName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString DepartureDate = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString DepartureTime = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString Seat = "";
};
