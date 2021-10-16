#include "CoreMinimal.h"
#include <string>
#include "stdint.h"
#include "DocsInfoStruct.generated.h"

#pragma once
UCLASS()
class FLIGHTATTENDANTSIM_API UDocsInfoStruct : public UObject
{
	GENERATED_BODY()

public:
	UDocsInfoStruct(uint64 PassengerId, std::string& LastName, std::string& FirstName, std::string& Nationality, std::string& BirthDate, std::string& ExpirationDate)
		: PassengerId(PassengerId), LastName(LastName), FirstName(FirstName), Nationality(Nationality), BirthDate(BirthDate), ExpirationDate(ExpirationDate) {  }
	UDocsInfoStruct() {}
	static TArray<UDocsInfoStruct*> Load(const char* PathToPassengersInfo);
	static TArray<UDocsInfoStruct*> LoadUsingFileHelper(const char* PathToPassengersInfo);
	static TArray<std::string> LoadUsingFileHelperStrings(const char* PathToPassengersInfo);
	static std::string GenerateRandomDate(int32 YearFrom, int32 YearTill);
	static UDocsInfoStruct* CreateInstance(
		const uint64 PassengerId, 
		const std::string& LastName, 
		const std::string& FirstName, 
		const std::string& Nationality, 
		const std::string& BirthDate, 
		const std::string& ExpirationDate);
	// uint64 is not supported by Blueprint
	UFUNCTION(BlueprintCallable)
	int32 GetPassengerId() const { return (int32)PassengerId; }
	UFUNCTION(BlueprintCallable)
	FString GetLastName() const { return FString(LastName.c_str()); }
	UFUNCTION(BlueprintCallable)
	FString GetFirstName() const { return FString(FirstName.c_str()); }
	UFUNCTION(BlueprintCallable)
	FString GetNationality() const { return FString(Nationality.c_str()); }
	UFUNCTION(BlueprintCallable)
	FString GetBirthDate() const { return FString(BirthDate.c_str()); }
	UFUNCTION(BlueprintCallable)
	FString GetExpirationDate() const { return FString(ExpirationDate.c_str()); }

private:
	const static int ColumnsCount = 6;

	uint64 PassengerId = 0;
	std::string LastName = "";
	std::string FirstName = "";
	std::string Nationality = "";
	std::string BirthDate = "";
	std::string ExpirationDate = "";

	static TArray<UDocsInfoStruct*> ParsePassengersInfo(std::string FileContent);
	static TArray<std::string> ParseStringFile(std::string FileContent);
};

