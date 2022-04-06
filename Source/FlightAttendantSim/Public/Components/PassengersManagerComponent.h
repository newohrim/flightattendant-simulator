// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Passengers/PassengerSpawnParams.h"
#include "Characters/Passengers/DocsInfo.h"
#include "Components/ActorComponent.h"
#include "PassengersManagerComponent.generated.h"

class AFABasePassenger;
class ULocationInfo;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UPassengersManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPassengersManagerComponent();
	
	UFUNCTION(BlueprintCallable)
	AFABasePassenger* CreatePassenger(const FTransform& PassengerTransform, ULocationInfo* Destination);

	AFABasePassenger* PopBufferedPassenger();

	void BufferPassengers(const TArray<FPassengerSpawnParams>& SpawnParams) { BufferedPassengers = SpawnParams; }

	const TArray<AFABasePassenger*>& GetSpawnedPassengers() const { return SpawnedPassengers; }

	FDocsInfo CreateDocument() const;
	
	void ClearRedundantPassengers();

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

	TArray<FPassengerSpawnParams> BufferedPassengers;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	AFABasePassenger* SpawnPassenger(const FPassengerSpawnParams& SpawnParams);

private:
	TArray<FString> LastNames;
	TArray<FString> FirstNames;
};
