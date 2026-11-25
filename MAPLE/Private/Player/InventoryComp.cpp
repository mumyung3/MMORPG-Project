// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/InventoryComp.h"
#include "Util/CustomLog.h"

UInventoryComp::UInventoryComp()
{
}

void UInventoryComp::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComp::InitializeComponent()
{
	Super::InitializeComponent();
}

void UInventoryComp::AddItem(const FItemData& PickedItem)
{
	// Checking If the Item is Already Exists in the Inventory
	FItemData* ExistingItem = Items.FindByPredicate([&](const FItemData& Existing) { return Existing.ItemID == PickedItem.ItemID; });

	if (ExistingItem) // Item that Already Exists
	{
		ExistingItem->Quantity += PickedItem.Quantity;
	}
	else // First Picked Item
	{
		Items.Add(PickedItem);
	}
}

bool UInventoryComp::RemoveItem(const FString& ItemID) // Need to Check
{
	/*
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemID == ItemID)
		{
			Items.RemoveAt(i);
			return true;
		}
	}
	return false;*/
	return true;
}

bool UInventoryComp::HasItem(const FString& ItemID)
{
	return Items.ContainsByPredicate([&](const FItemData& Existing) { return Existing.ItemID == ItemID; });
}
