#include "DocsInfoStruct.h"
#include "Misc/Paths.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "stdint.h"

// DEPRECATED (USE LoadUsingFileHelper)
TArray<UDocsInfoStruct*> UDocsInfoStruct::Load(const char* PathToPassengersInfo)
{
	TArray<UDocsInfoStruct*> result;
	std::stringstream temp;
	temp << TCHAR_TO_UTF8(*FPaths::ProjectDir()) << PathToPassengersInfo;
	UE_LOG(LogTemp, Warning, TEXT("PathToPassengersInfo: %s"), *FString(temp.str().c_str()))
	std::ifstream InputStream(temp.str());
	std::string line;
	std::vector<std::string> parts(ColumnsCount);
	while (std::getline(InputStream, line)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Got line: %s"), *FString(line.c_str()))
		std::stringstream linestream(line);
		int count = 0;
		while (std::getline(linestream, parts[count++], ';'));
		getline(linestream, parts[count++]);
		
		UDocsInfoStruct* created_doc= nullptr;
		NewObject<UDocsInfoStruct>(created_doc, UDocsInfoStruct::StaticClass());
		if (created_doc) 
		{
			result.Add(created_doc);
		}
	}
	//delete[] parts;
	return result;
}

TArray<UDocsInfoStruct*> UDocsInfoStruct::LoadUsingFileHelper(const char* PathToPassengersInfo)
{
	FString file = FPaths::ProjectConfigDir();
	file.Append(UTF8_TO_TCHAR(PathToPassengersInfo));

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	file = FileManager.GetFilenameOnDisk(*file);

	FString FileContent;
	if (FileManager.FileExists(*file))
	{
		if (FFileHelper::LoadFileToString(FileContent, *file, FFileHelper::EHashOptions::None))
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Text From File: %s"), *FileContent);
			return ParsePassengersInfo(TCHAR_TO_UTF8(*FileContent));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
	}
	return TArray<UDocsInfoStruct*>();
}

TArray<FString> UDocsInfoStruct::LoadUsingFileHelperStrings(const char* PathToPassengersInfo)
{
	FString file = FPaths::ProjectConfigDir();
	file.Append(UTF8_TO_TCHAR(PathToPassengersInfo));

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	file = FileManager.GetFilenameOnDisk(*file);

	FString FileContent;
	if (FileManager.FileExists(*file))
	{
		if (FFileHelper::LoadFileToString(FileContent, *file, FFileHelper::EHashOptions::None))
		{
			return ParseStringFile(TCHAR_TO_UTF8(*FileContent));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
	}
	return TArray<FString>();
}

FString UDocsInfoStruct::GenerateRandomDate(int32 YearFrom, int32 YearTill)
{
	return GenerateRandomDateExtended(1, 30, 1, 12, YearFrom, YearTill);
}

FString UDocsInfoStruct::GenerateRandomDateExtended(int32 DayFrom, int32 DayTill, int32 MonthFrom, int32 MonthTill, int32 YearFrom, int32 YearTill)
{
	std::stringstream date;
	date << FMath::RandRange(DayFrom, DayTill) << '.'
		<< FMath::RandRange(MonthFrom, MonthTill) << '.'
		<< FMath::RandRange(YearFrom, YearTill);
	return FString(date.str().c_str());
}

FString UDocsInfoStruct::AddDaysToDate(const FString& Date, int32 Days)
{
	FString DayStr;
	FString MonthStr;
	FString YearStr;
	FString Temp;
	check(Date.Split(".", &DayStr, &Temp));
	check(Temp.Split(".", &MonthStr, &YearStr));
	int32 Day = FCString::Atoi(*DayStr) + Days;
	int32 Month = FCString::Atoi(*MonthStr);
	int32 Year = FCString::Atoi(*YearStr);
	while (Day > 30) 
	{
		++Month;
		if (Month > 12) 
		{
			++Year;
			Month = 1;
		}
		Day -= 30;
	}
	while (Day < 0) 
	{
		--Month;
		if (Month < 1) 
		{
			Month = 12;
			--Year;
		}
		Day += 30;
	}
	return FString::FromInt(Day)
		.AppendChar('.')
		.Append(Month > 9 ? FString::FromInt(Month) : FString("0").Append(FString::FromInt(Month)))
		.AppendChar('.')
		.Append(FString::FromInt(Year));
}

FString UDocsInfoStruct::GenerateRandomTime()
{
	int32 Hours = FMath::RandRange(0, 23);
	int32 Minutes = FMath::RandRange(0, 59);
	std::stringstream date;
	date << Hours << ':';
	if (Minutes <= 9)
		date << '0';
	date << Minutes;
	return FString(date.str().c_str());
}

UDocsInfoStruct* UDocsInfoStruct::CreateInstance(
	const uint64 PassengerId,
	const FString& LastName,
	const FString& FirstName,
	const FString& Nationality,
	const FString& BirthDate,
	const FString& ExpirationDate)
{
	UDocsInfoStruct* CreatedInstance = NewObject<UDocsInfoStruct>(UDocsInfoStruct::StaticClass());
	CreatedInstance->PassengerId = PassengerId;
	CreatedInstance->LastName = LastName;
	CreatedInstance->FirstName = FirstName;
	CreatedInstance->Nationality = Nationality;
	CreatedInstance->BirthDate = BirthDate;
	CreatedInstance->ExpirationDate = ExpirationDate;

	return CreatedInstance;
}

UDocsInfoStruct* UDocsInfoStruct::CopyInstance(const UDocsInfoStruct* OriginalInstance)
{
	return UDocsInfoStruct::CreateInstance(
		OriginalInstance->GetPassengerId(),
		OriginalInstance->GetLastName(),
		OriginalInstance->GetFirstName(),
		OriginalInstance->GetNationality(),
		OriginalInstance->GetBirthDate(),
		OriginalInstance->GetExpirationDate());
}

TArray<UDocsInfoStruct*> UDocsInfoStruct::ParsePassengersInfo(std::string FileContent)
{
	TArray<UDocsInfoStruct*> result;
	std::stringstream InputStream(FileContent);
	std::string line;
	std::vector<std::string> parts(ColumnsCount);
	while (std::getline(InputStream, line))
	{
		UE_LOG(LogTemp, Warning, TEXT("Got line: %s"), *FString(line.c_str()))
			std::stringstream linestream(line);
		int count = 0;
		while (std::getline(linestream, parts[count++], ';'));
		getline(linestream, parts[count++]);

		UDocsInfoStruct* created_doc = NewObject<UDocsInfoStruct>(UDocsInfoStruct::StaticClass());
		created_doc->PassengerId = std::stoi(parts[0]);
		created_doc->LastName = FString(parts[1].c_str());
		created_doc->FirstName = FString(parts[2].c_str());
		created_doc->Nationality = FString(parts[3].c_str());
		created_doc->BirthDate = FString(parts[4].c_str());
		created_doc->ExpirationDate = FString(parts[5].c_str());
		//NewObject<UDocsInfoStruct>(created_doc, UDocsInfoStruct::StaticClass());
		if (created_doc)
		{
			result.Add(created_doc);
		}
	}
	//delete[] parts;
	UE_LOG(LogTemp, Display, TEXT("Total passengers parsed: %d."), result.Num());
	return result;
}

TArray<FString> UDocsInfoStruct::ParseStringFile(std::string FileContent)
{
	TArray<FString> result;
	std::stringstream InputStream(FileContent);
	std::string line;
	std::vector<std::string> parts(ColumnsCount);
	while (std::getline(InputStream, line))
	{
		//std::string* temp = new std::string(line);
		result.Add(FString(line.c_str()));
	}
	//delete[] parts;
	UE_LOG(LogTemp, Display, TEXT("Total strings parsed: %d."), result.Num());
	return result;
}
