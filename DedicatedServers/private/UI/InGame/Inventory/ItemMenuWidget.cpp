// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGame/Inventory/ItemMenuWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Inventory/Manger/UseItemComponent.h"

void UItemMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	UCanvasPanelSlot* ParentSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SizeBox_Item);
	if (ParentSlot)
	{
		ParentSlot->SetPosition(MousePos);
	}

	Button_Close->OnClicked.AddDynamic(this, &UItemMenuWidget::OnClose);
	Button_Use->OnClicked.AddDynamic(this, &UItemMenuWidget::OnUseItem);
	Button_Drop->OnClicked.AddDynamic(this, &UItemMenuWidget::OnDropped);
}
void UItemMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnClose();
}

void UItemMenuWidget::OnClose()
{
	this->RemoveFromParent();
}

void UItemMenuWidget::OnUseItem()
{
	// 굳이 slot위젯에서 실행안해도됨. 어차피 실제 데이터는 ALLITEM 배열
	// 실제 기능 호출
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		APawn* MyPawn = PC->GetPawn();
		Cast<AWarriorHeroCharacter>(MyPawn)->PickUpComp->UseItem(ItemIndex, ItemType);	
	}
	//안사라지는 효과 있음.
	this->RemoveFromParent();
}

void UItemMenuWidget::OnDropped()
{
	
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		APawn* MyPawn = PC->GetPawn();
		Cast<AWarriorHeroCharacter>(MyPawn)->PickUpComp->OnDrop(ItemIndex, ItemType);
	}
	//안사라지는 효과 있음.
	this->RemoveFromParent();
}
