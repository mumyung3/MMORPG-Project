// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "ItemInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_ItemName;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UImage> Image_ICON;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Power;
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Description;

	UPROPERTY(EditAnywhere, Category = "data")
	TArray<UTexture2D*> Icon;

	UPROPERTY(EditAnywhere, Category = "data")
	FSlot Item;

	UFUNCTION()
	void SetItemInfo();
	
	UFUNCTION()
	void HiddenInfo();
	
};
