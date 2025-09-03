// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGame/Inventory/ItemInfoWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
void UItemInfoWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetItemInfo();
	
}
void UItemInfoWidget::SetItemInfo()
{

	// 조심해야할 조건문..
	if (Item.ItemTableRowHandle.DataTable && Item.ItemTableRowHandle.RowName != NAME_None)
	{

		check(Item.ItemTableRowHandle.DataTable)

			FItem* ItemData = Item.ItemTableRowHandle.DataTable->FindRow<FItem>(Item.ItemTableRowHandle.RowName, TEXT(""));
		TextBlock_ItemName->SetText(FText::FromString(ItemData->Name));
		TextBlock_Power->SetText(FText::AsNumber(ItemData->Power));
		TextBlock_Description->SetText(FText::FromString(ItemData->Description));

		if (Icon.IsEmpty())
			return;

		check(IsValid(Image_ICON))

			switch (Item.ItemType)
		{

			case EItemType::Sword:
				// 여기도 데이터 에셋으로 처리해볼까? 차후
				Image_ICON->SetBrushFromTexture(Icon[0]);
				break;
			case EItemType::Shield:
				Image_ICON->SetBrushFromTexture(Icon[1]);
				break;
			case EItemType::Eatables:
				Image_ICON->SetBrushFromTexture(Icon[2]);
				break;
			case EItemType::None:
				UE_LOG(LogTemp, Display, TEXT("Item type is None"));
				break;
		}

		if (Item.Quantity > 0) this->SetVisibility(ESlateVisibility::Visible);

	}
}
void UItemInfoWidget::HiddenInfo(){
	this->SetVisibility(ESlateVisibility::Hidden);
}