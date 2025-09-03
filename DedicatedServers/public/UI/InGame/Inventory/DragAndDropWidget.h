// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "DragAndDropWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDragAndDropWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_Thumnail;

	UPROPERTY()
	FSlot SlotItem;

	void InitImage();
protected:
	virtual void NativePreConstruct() override;
};
