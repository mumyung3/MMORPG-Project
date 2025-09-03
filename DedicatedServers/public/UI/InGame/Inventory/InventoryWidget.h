// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Money;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Sword;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Shield;
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UButton> Button_Eatable;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UWrapBox> WrapBox_Sword;
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UWrapBox> WrapBox_Shield;
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UWrapBox> WrapBox_Eatable;
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UItemInfoWidget> ItemInfo;
	
	UFUNCTION()
	void SwitchToPageSword();
	UFUNCTION()
	void SwitchToPageShield();
	UFUNCTION()
	void SwitchToPageEatable();

	UFUNCTION(BlueprintImplementableEvent)
	void SetColorSwordButton();

	UFUNCTION(BlueprintImplementableEvent)
	void SetColorShieldButton();

	UFUNCTION(BlueprintImplementableEvent)
	void SetColorEatableButton();
protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION()
	void UpdateItemsInInventoryUI(const FALLItem& AllItemData);

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class USlotWidget> SlotWidgetClass;

protected:
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;

	
};
