// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGame/InGameOverlay.h"

#include "Components/TextBlock.h"
#include "UI/InGame/Inventory/InventoryWidget.h"
void UInGameOverlay::SetMoney(int32 Money)
{

	Inventory->TextBlock_Money->SetText(FText::FromString(FString::FromInt(Money)));
	
}