// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemsInventoryComponent.h"


// Sets default values for this component's properties
UItemsInventoryComponent::UItemsInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

inline int32 UItemsInventoryComponent::GetItemsCount(const TEnumAsByte<EInventoryItemType> ItemType) const
{
	if (Items.IsValidIndex(ItemType))
	{
		return Items[ItemType];
	}
	
	return 0;
}

inline void UItemsInventoryComponent::AddItem(const TEnumAsByte<EInventoryItemType> ItemType, const int32 Count)
{
	if (Items.IsValidIndex(ItemType) && Count > 0)
	{
		Items[ItemType] += Count;
	}
}

inline bool UItemsInventoryComponent::RemoveItems(const TEnumAsByte<EInventoryItemType> ItemType, const int32 Count)
{
	if (Items.IsValidIndex(ItemType) && (Count > 0) && (Items[ItemType] - Count >= 0))
	{
		Items[ItemType] -= Count;
		return true;
	}

	return false;
}

void UItemsInventoryComponent::InitializeSpace()
{
	Items.AddDefaulted(INVENTORY_ITEMS_NR);
	for (int32 i = 0; i < Items.Num(); ++i)
		Items[i] = 0;
}

void UItemsInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsPreLoaded)
	{
		InitializeSpace();
	}
}

