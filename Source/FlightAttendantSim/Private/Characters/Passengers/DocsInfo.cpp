// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Passengers/DocsInfo.h"

FString FDocsInfo::GenerateId()
{
	TStringBuilder<15> StringBuilder;
	for (int i = 0; i < 3; ++i)
		StringBuilder << FString::Chr(static_cast<TCHAR>(FMath::RandRange('A', 'Z')));
	StringBuilder << ' ';
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			StringBuilder << FMath::RandHelper(10);
		if (i < 2)
			StringBuilder << '.';
	}
	return StringBuilder.ToString();
}
