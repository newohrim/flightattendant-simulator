// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DocsInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDocsInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString LastName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString FirstName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString PassengerId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsWanted = false;

	static FString GenerateId();
};
