// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemsInventoryComponent.generated.h"

UENUM(BlueprintType)
enum EInventoryItemType
{
	Hypercharge		UMETA(DisplayName = "Hypercharge"),
	DefaultFood		UMETA(DisplayName = "DefaultFood"),

	// Last element to always know how many different enum values are possible
	INVENTORY_ITEMS_NR
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FLIGHTATTENDANTSIM_API UItemsInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool IsPreLoaded = false;
	
	// Sets default values for this component's properties
	UItemsInventoryComponent();

	UFUNCTION(BlueprintCallable)
	int32 GetItemsCount(const TEnumAsByte<EInventoryItemType> ItemType) const;

	UFUNCTION(BlueprintCallable)
	void AddItem(const TEnumAsByte<EInventoryItemType> ItemType, const int32 Count = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveItems(const TEnumAsByte<EInventoryItemType> ItemType, const int32 Count = 1);

	void InitializeSpace();

protected:
	TArray<int32> Items;
	
	virtual void BeginPlay() override;
};
