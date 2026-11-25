// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "SlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
public:
	
	// 데이터 테이블 세팅 필요 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "data")
	FSlot ItemSlot{};

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "data")
	int32 Index;
	
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Quantity;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UButton> Button_Slot;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class USizeBox> SizeBox_Quantity;
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<class UImage> Image_Item;

	UFUNCTION()
	void ShowItemInfo();

	UFUNCTION()
	void HiddenItemInfo();

	UFUNCTION()
	void UseItem();

protected:
	virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation ) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UDragAndDropWidget> DragAndDropImageWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UDragAndDropWidget> DragAndDropImageWidget;

protected:
 virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;

public:
	bool InitInfo();

};
