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
	UPROPERTY(BlueprintReadOnly)
	FString LastName;
	UPROPERTY(BlueprintReadOnly)
	FString FirstName;
	UPROPERTY(BlueprintReadOnly)
	int32 PassengerId;
};
