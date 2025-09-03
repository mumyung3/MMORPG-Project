// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGame/Inventory/DragAndDropWidget.h"

#include "Components/Image.h"

void UDragAndDropWidget::NativePreConstruct()
{
	InitImage();
	Super::NativePreConstruct();
}

void UDragAndDropWidget::InitImage()
{
	if (SlotItem.ItemTableRowHandle.RowName.IsNone() || SlotItem.Quantity <= 0 || SlotItem.ItemType == EItemType::None) return;

	FItem* Item = SlotItem.ItemTableRowHandle.DataTable->FindRow<FItem>(SlotItem.ItemTableRowHandle.RowName, TEXT(""));
	Image_Thumnail->SetBrushFromTexture(Item->Thumbnail);

}