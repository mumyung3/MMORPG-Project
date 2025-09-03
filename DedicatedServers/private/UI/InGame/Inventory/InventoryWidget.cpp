// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGame/Inventory/InventoryWidget.h"

#include "Character/WarriorHeroCharacter.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "Inventory/Manger/UseItemComponent.h"
#include "UI/InGame/Inventory/DragDropOperationItem.h"
#include "UI/InGame/Inventory/ItemInfoWidget.h"
#include "UI/InGame/Inventory/SlotWidget.h"
#include "Util/CustomLog.h"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	check(IsValid(WidgetSwitcher));
	check(SlotWidgetClass);

	WidgetSwitcher->SetActiveWidgetIndex(0);

	Button_Sword->OnClicked.AddDynamic(this, &UInventoryWidget::SwitchToPageSword);
	Button_Shield->OnClicked.AddDynamic(this, &UInventoryWidget::SwitchToPageShield);
	Button_Eatable->OnClicked.AddDynamic(this, &UInventoryWidget::SwitchToPageEatable);

	ItemInfo->SetVisibility(ESlateVisibility::Hidden);
}
void UInventoryWidget::SwitchToPageSword()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}
void UInventoryWidget::SwitchToPageShield()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}
void UInventoryWidget::SwitchToPageEatable()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
}
FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{

	const FKey Key = InKeyEvent.GetKey();

	LOGA("Press I")
	// "I" 키를 눌렀을 경우 처리
	if (Key == EKeys::I)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			// 인벤토리 닫기와 같은 동작 처리
			ESlateVisibility CurrentVis = this->GetVisibility();
			ESlateVisibility NewVis = (CurrentVis == ESlateVisibility::Visible) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
			this->SetVisibility(NewVis);

			// 마우스 커서 및 입력 모드 설정
			if (NewVis == ESlateVisibility::Visible)
			{
				// UI 모드로 설정, 마우스 커서 표시
				PC->SetInputMode(FInputModeUIOnly());
				PC->bShowMouseCursor = true;
			}
			else
			{
				// 게임 모드로 설정, 마우스 커서 숨기기
				PC->SetInputMode(FInputModeGameOnly());
				PC->bShowMouseCursor = false;
			}
			return FReply::Handled(); // 이벤트 처리 완료
		}
	}

	if (Key == EKeys::One) // 숫자 1
	{
		SwitchToPageSword();
		SetColorSwordButton();
		return FReply::Handled();
	}
	else if (Key == EKeys::Two) // 숫자 2
	{
		SwitchToPageShield();
		SetColorShieldButton();
		return FReply::Handled();
	}
	else if (Key == EKeys::Three) // 숫자 3
	{
		SwitchToPageEatable();
		SetColorEatableButton();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInventoryWidget::UpdateItemsInInventoryUI(const FALLItem& AllItemData)
{

	WrapBox_Sword->ClearChildren();
	WrapBox_Shield->ClearChildren();
	WrapBox_Eatable->ClearChildren();

	for (int32 i = 0; i < AllItemData.Swords.Num(); ++i)
	{
		USlotWidget* SlotWidget = CreateWidget<USlotWidget>(this, SlotWidgetClass);
		// 여기서 ref로 해도 괜찮을 듯함.
		SlotWidget->ItemSlot = AllItemData.Swords[i];
		SlotWidget->Index = i;
		SlotWidget->InitInfo();
		WrapBox_Sword->AddChildToWrapBox(SlotWidget);
	}

	for (int32 i = 0; i < AllItemData.Shields.Num(); ++i)
	{
		USlotWidget* SlotWidget = CreateWidget<USlotWidget>(GetOwningPlayer(), SlotWidgetClass);
		// 여기서 ref로 해도 괜찮을 듯함.
		SlotWidget->ItemSlot = AllItemData.Shields[i];
		SlotWidget->Index = i;
		SlotWidget->InitInfo();
		WrapBox_Shield->AddChildToWrapBox(SlotWidget);
	}

	for (int32 i = 0; i < AllItemData.Eatables.Num(); ++i)
	{
		USlotWidget* SlotWidget = CreateWidget<USlotWidget>(GetOwningPlayer(), SlotWidgetClass);
		// 여기서 ref로 해도 괜찮을 듯함.
		SlotWidget->ItemSlot = AllItemData.Eatables[i];
		SlotWidget->Index = i;
		SlotWidget->InitInfo();
		WrapBox_Eatable->AddChildToWrapBox(SlotWidget);
	}
}
bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation){

	UDragDropOperationItem* DragDropOperation = Cast<UDragDropOperationItem>(InOperation);

	if (DragDropOperation)
	{
		FSlot SourceItem = DragDropOperation->Item;
		int32 SourceIndex =	DragDropOperation->Index;
		// 해당 기능은 컴포넌트에서 처리하자
		APlayerController* PC = GetOwningPlayer();
		if (PC)
		{
			APawn* MyPawn = PC->GetPawn();
			Cast<AWarriorHeroCharacter>(MyPawn)->PickUpComp->Drop(SourceItem);
			
		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}