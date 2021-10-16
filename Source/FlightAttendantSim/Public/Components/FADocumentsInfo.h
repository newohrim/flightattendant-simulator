// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FADocumentsInfo.generated.h"

class UDocsInfoStruct;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLIGHTATTENDANTSIM_API UFADocumentsInfo : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFADocumentsInfo();
	UFUNCTION(BlueprintCallable)
	void SetDocumentsInfo(UDocsInfoStruct* Info);
	UFUNCTION(BlueprintCallable)
	UDocsInfoStruct* GetDocsInfo();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UDocsInfoStruct* DocsInfo;
};
