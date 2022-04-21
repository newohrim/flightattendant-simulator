// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Passengers/PassengerSpawnParams.h"
#include "Characters/Passengers/DocsInfo.h"
#include "Components/ActorComponent.h"
#include "PassengersManagerComponent.generated.h"

class AFABasePassenger;
class ULocationInfo;
class UMapNode;

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

	FDocsInfo CreateDocument();
	
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
	UPROPERTY(EditDefaultsOnly, Category = "DocsInfoConstraints")
	int32 MaxWantedCharactersNum = 2;
	UPROPERTY(EditDefaultsOnly, Category = "DocsInfoConstraints")
	float WantedCharacterSpawnRate = 0.2f;
	UPROPERTY(EditDefaultsOnly, Category = "DocsInfoConstraints")
	float WantedCharacterTransportationPenaltyRate = 0.25f;
	UPROPERTY(EditDefaultsOnly, Category = "DocsInfoConstraints")
	int32 WantedCharacterTransportationPenalty = 120;
	UPROPERTY(EditDefaultsOnly, Category = "DocsInfoConstraints")
	int32 WantedCharacterTransportationDelayMin = 10;
	UPROPERTY(EditDefaultsOnly, Category = "DocsInfoConstraints")
	int32 WantedCharacterTransportationDelayMax = 300;
	UPROPERTY(EditDefaultsOnly, Category = "PassengersSpawn")
	TSubclassOf<AFABasePassenger> PassengerToSpawn;
	UPROPERTY(EditAnywhere, Category = "Waypoints")
	FString LeavePlaneWaypoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Passengers Economics")
	int32 PassengerTicketPrice = 15;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Passengers Economics")
	int32 PassengerServePrice = 5;
	UPROPERTY(EditDefaultsOnly)
	FText SuspendMessageTemplate;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AFABasePassenger*> SpawnedPassengers;

	TArray<FPassengerSpawnParams> BufferedPassengers;

	TArray<FDocsInfo> WantedCharacters;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	AFABasePassenger* SpawnPassenger(const FPassengerSpawnParams& SpawnParams);

	UFUNCTION()
	virtual void LocationLoadedHandle(const UMapNode* Destination);

	UFUNCTION()
	virtual void PlayerArrivedHandle(UMapNode* Destination);

	void ExecutePenalties() const;

	void PassengersLeavePlane(const UMapNode* Destination);

private:
	TArray<FString> LastNames;
	TArray<FString> FirstNames;
};
