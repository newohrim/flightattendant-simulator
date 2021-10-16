#include "DocsInfoStruct.h"
#include "Misc/Paths.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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

TArray<std::string> UDocsInfoStruct::LoadUsingFileHelperStrings(const char* PathToPassengersInfo)
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
	return TArray<std::string>();
}

std::string UDocsInfoStruct::GenerateRandomDate(int32 YearFrom, int32 YearTill)
{
	std::stringstream date;
	date << FMath::RandRange(1, 30) << '.' 
		<< FMath::RandRange(1, 12) << '.' 
		<< FMath::RandRange(YearFrom, YearTill);
	return date.str();
}

UDocsInfoStruct* UDocsInfoStruct::CreateInstance(
	const uint64 PassengerId, 
	const std::string& LastName, 
	const std::string& FirstName, 
	const std::string& Nationality, 
	const std::string& BirthDate, 
	const std::string& ExpirationDate)
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
		created_doc->PassengerId = std::stoull(parts[0]);
		created_doc->LastName = parts[1];
		created_doc->FirstName = parts[2];
		created_doc->Nationality = parts[3];
		created_doc->BirthDate = parts[4];
		created_doc->ExpirationDate = parts[5];
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

TArray<std::string> UDocsInfoStruct::ParseStringFile(std::string FileContent)
{
	TArray<std::string> result;
	std::stringstream InputStream(FileContent);
	std::string line;
	std::vector<std::string> parts(ColumnsCount);
	while (std::getline(InputStream, line))
	{
		//std::string* temp = new std::string(line);
		result.Add(line);
	}
	//delete[] parts;
	UE_LOG(LogTemp, Display, TEXT("Total strings parsed: %d."), result.Num());
	return result;
}
