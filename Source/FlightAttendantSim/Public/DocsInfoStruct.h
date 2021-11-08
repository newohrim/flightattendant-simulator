#include "CoreMinimal.h"
#include <string>
#include "DocsInfoStruct.generated.h"

#pragma once

UCLASS(BlueprintType)
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
	UFUNCTION(BlueprintCallable)
	static UDocsInfoStruct* CopyInstance(const UDocsInfoStruct* OriginalInstance);
	// uint64 is not supported by Blueprint
	UFUNCTION(BlueprintCallable)
	int32 GetPassengerId() const { return PassengerId; }
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

protected:
	const static int ColumnsCount = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	int32 PassengerId = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString LastName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString FirstName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString Nationality = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString BirthDate = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DocsInfo");
	FString ExpirationDate = "";

	static TArray<UDocsInfoStruct*> ParsePassengersInfo(std::string FileContent);
	static TArray<FString> ParseStringFile(std::string FileContent);
};
