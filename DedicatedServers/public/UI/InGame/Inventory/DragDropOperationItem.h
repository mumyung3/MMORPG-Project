// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "DragDropOperationItem.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDragDropOperationItem : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FSlot Item{};
	UPROPERTY(BlueprintReadOnly)
	int32 Index{};
};
