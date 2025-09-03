// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGame/Inventory/SlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Inventory/Manger/UseItemComponent.h"
#include "Kismet/KismetInputLibrary.h"
#include "UI/InGame/InGameHUDBase.h"
#include "UI/InGame/InGameOverlay.h"
#include "UI/InGame/Inventory/DragDropOperationItem.h"
#include "UI/InGame/Inventory/InventoryWidget.h"
#include "UI/InGame/Inventory/ItemInfoWidget.h"
#include "UI/InGame/Inventory/DragAndDropWidget.h"
#include "DataTypes/Inventory/InventoryTypes.h"
bool USlotWidget::InitInfo()
{
	if (ItemSlot.Quantity <= 0)
	{
		SizeBox_Quantity->SetVisibility(ESlateVisibility::Hidden);
		Image_Item->SetVisibility(ESlateVisibility::Hidden);
		// 실제로는 빈 슬롯을 넣어야 하는데, 차후 보기.
		ItemSlot.ItemType = EItemType::None;
	}
	else
	{

		TextBlock_Quantity->SetText(FText::AsNumber(ItemSlot.Quantity));

		if (ItemSlot.ItemTableRowHandle.RowName.IsNone())
			return true;

		FItem* Item = ItemSlot.ItemTableRowHandle.GetRow<FItem>(*ItemSlot.ItemTableRowHandle.RowName.ToString());
		if (Item)
		{
			Image_Item->SetBrushFromTexture(Item->Thumbnail);
		}
	}
	return false;
}
void USlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (InitInfo()) return;
}
void USlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitInfo();
	Button_Slot->OnHovered.AddDynamic(this,&USlotWidget::ShowItemInfo);
	Button_Slot->OnUnhovered.AddDynamic(this, &USlotWidget::HiddenItemInfo);
	Button_Slot->OnClicked.AddDynamic(this, &USlotWidget::UseItem);
}

void USlotWidget::ShowItemInfo()
{
	// UInventoryWidget* Inventory = GetTypedOuter<UInventoryWidget>();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AHUD* HUD = PC->GetHUD();
		if (AInGameHUDBase* InGameHUD = Cast<AInGameHUDBase>(HUD))
		{
			InGameHUD->InGameOverlay->Inventory->ItemInfo->Item = ItemSlot;
			InGameHUD->InGameOverlay->Inventory->ItemInfo->SetItemInfo();
		}
	}
}

void USlotWidget::HiddenItemInfo()
{
	// UInventoryWidget* Inventory = GetTypedOuter<UInventoryWidget>();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AHUD* HUD = PC->GetHUD();
		if (AInGameHUDBase* InGameHUD = Cast<AInGameHUDBase>(HUD))
		{
			InGameHUD->InGameOverlay->Inventory->ItemInfo->HiddenInfo();
		}
	}
}
void USlotWidget::UseItem()
{

	// 빈 슬롯은 막아야함! fatal error nullptr

	if (ItemSlot.Quantity <= 0 || ItemSlot.ItemTableRowHandle.RowName.IsNone())
		return;

	FItem* ItemData = ItemSlot.ItemTableRowHandle.DataTable->FindRow<FItem>(ItemSlot.ItemTableRowHandle.RowName, TEXT(""));
	int32  Power = ItemData->Power;

	// 실제 기능 호출
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		APawn* MyPawn = PC->GetPawn();
		Cast<AWarriorHeroCharacter>(MyPawn)->PickUpComp->UseItem(Index, ItemSlot.ItemType);
	}
}

FReply USlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::LeftMouseButton))
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return EventReply.NativeReply;
	}
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}
void USlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	// 없는 슬롯이 움직이긴 그럼.
	if (ItemSlot.Quantity <= 0 || ItemSlot.ItemTableRowHandle.RowName.IsNone())
		return;

	// 나의 슬롯을 빼줘. 컴포넌트
	//  실제 기능 호출
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		APawn* MyPawn = PC->GetPawn();
		Cast<AWarriorHeroCharacter>(MyPawn)->PickUpComp->DragStart(Index, ItemSlot.ItemType);

		// 드래그 연산
		UDragDropOperationItem* DragDropOp = NewObject<UDragDropOperationItem>();

		// 변수로 갖고 있으면 안되려나?
		DragAndDropImageWidget = CreateWidget<UDragAndDropWidget>(this, DragAndDropImageWidgetClass);
		DragAndDropImageWidget->SlotItem = ItemSlot;
		DragAndDropImageWidget->InitImage();
		DragDropOp->Item = ItemSlot;
		DragDropOp->Index = Index;
		DragDropOp->DefaultDragVisual = Cast<UUserWidget>(DragAndDropImageWidget);

		OutOperation = DragDropOp; // 출력 파라미터에 할당
	}
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}
bool USlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation){

	UDragDropOperationItem* DragDropOperation = Cast<UDragDropOperationItem>(InOperation);

	if (DragDropOperation)
	{
		FSlot SourceItem = DragDropOperation->Item;
		int32 SourceIndex =	DragDropOperation->Index;
		int32 DestinationIndex = Index;
		// 해당 기능은 컴포넌트에서 처리하자
		APlayerController* PC = GetOwningPlayer();
		if (PC)
		{
			APawn* MyPawn = PC->GetPawn();
			Cast<AWarriorHeroCharacter>(MyPawn)->PickUpComp->DragEnd(SourceItem, SourceIndex, DestinationIndex);

			//버블링 현상 막음.
			return true;
		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}