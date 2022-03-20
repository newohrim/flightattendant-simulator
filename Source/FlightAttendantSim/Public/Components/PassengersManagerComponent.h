// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PassengersManagerComponent.generated.h"

class AFABasePassenger;
class ULocationInfo;
struct FDocsInfo;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UPassengersManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPassengersManagerComponent();
	
	UFUNCTION(BlueprintCallable)
	AFABasePassenger* CreatePassenger(const FTransform& PassengerTransform, ULocationInfo* Destination);

	FDocsInfo CreateDocument() const;
	
	void ClearPassengers();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToLastNames;
	UPROPERTY(EditDefaultsOnly, Category = "FilePaths")
	FString PathToFirstNames;
	UPROPERTY(EditDefaultsOnly, Category = "DocsInfoConstraints")
	int32 MinPassengerId;
	UPROPERTY(EditDefaultsOnly, Category = "DocsInfoConstraints")
	int32 MaxPassengerId;
	UPROPERTY(EditDefaultsOnly, Category = "PassengersSpawn")
	TSubclassOf<AFABasePassenger> PassengerToSpawn;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AFABasePassenger*> SpawnedPassengers;
	
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TArray<FString> LastNames;
	TArray<FString> FirstNames;
};
