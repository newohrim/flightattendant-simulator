#include "CoreMinimal.h"
#include <string>
#include "DocsInfoStruct.generated.h"

#pragma once
UCLASS()
class FLIGHTATTENDANTSIM_API UDocsInfoStruct : public UObject
{
	GENERATED_BODY()

public:
	UDocsInfoStruct() {}
	static TArray<UDocsInfoStruct*> Load(const char* PathToPassengersInfo);
	static TArray<UDocsInfoStruct*> LoadUsingFileHelper(const char* PathToPassengersInfo);
	static TArray<FString> LoadUsingFileHelperStrings(const char* PathToPassengersInfo);
	static FString GenerateRandomDate(int32 YearFrom, int32 YearTill);
	static UDocsInfoStruct* CreateInstance(
		const uint64 PassengerId, 
		const FString& LastName,
		const FString& FirstName,
		const FString& Nationality,
		const FString& BirthDate,
		const FString& ExpirationDate);
	// uint64 is not supported by Blueprint
	UFUNCTION(BlueprintCallable)
	int32 GetPassengerId() const { return (int32)PassengerId; }
	UFUNCTION(BlueprintCallable)
	FString GetLastName() const { return LastName; }
	UFUNCTION(BlueprintCallable)
	FString GetFirstName() const { return FirstName; }
	UFUNCTION(BlueprintCallable)
	FString GetNationality() const { return Nationality; }
	UFUNCTION(BlueprintCallable)
	FString GetBirthDate() const { return BirthDate; }
	UFUNCTION(BlueprintCallable)
	FString GetExpirationDate() const { return ExpirationDate; }

private:
	const static int ColumnsCount = 6;

	uint64 PassengerId = 0;
	FString LastName = "";
	FString FirstName = "";
	FString Nationality = "";
	FString BirthDate = "";
	FString ExpirationDate = "";

	static TArray<UDocsInfoStruct*> ParsePassengersInfo(std::string FileContent);
	static TArray<FString> ParseStringFile(std::string FileContent);
};

