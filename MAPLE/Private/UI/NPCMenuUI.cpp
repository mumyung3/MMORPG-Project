// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NPCMenuUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Util/CustomLog.h"

void UNPCMenuUI::OnShopClicked() { ButtonToActionMap[ENPCMenu::Shop](); }

void UNPCMenuUI::OnUpgradeClicked() { ButtonToActionMap[ENPCMenu::Upgrade](); }

void UNPCMenuUI::OnTalkClicked() { ButtonToActionMap[ENPCMenu::Talk](); }

void UNPCMenuUI::OnExitClicked() { ButtonToActionMap[ENPCMenu::Exit](); }

void UNPCMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// ** 버튼마다 실행될 함수 등록 *
	ButtonToActionMap.Add(ENPCMenu::Shop, [this]() { LOGC("SHOP") });
	ButtonToActionMap.Add(ENPCMenu::Upgrade, [this]() { LOGC("UPGRADE") });
	
	ButtonToActionMap.Add(ENPCMenu::Talk, [this]() {
		if (OnTalkButton)
			RemoveFromParent();
			OnTalkButton();
	});

	ButtonToActionMap.Add(ENPCMenu::Exit, [this]() { // 나가기 버튼 클릭 시 실행
		if (OnExitButton)
		{
			RemoveFromParent();
			OnExitButton();
		}
	});

	// ** 버튼 클릭 이벤트 바인딩 *
	ShopButton->OnClicked.AddDynamic(this, &UNPCMenuUI::OnShopClicked);
	UpgradeButton->OnClicked.AddDynamic(this, &UNPCMenuUI::OnUpgradeClicked);
	TalkButton->OnClicked.AddDynamic(this, &UNPCMenuUI::OnTalkClicked);
	ExitButton->OnClicked.AddDynamic(this, &UNPCMenuUI::OnExitClicked);
}

void UNPCMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
}

void UNPCMenuUI::SetNPCName(const FText& InNPCName)
{
	NPCNameText->SetText(InNPCName);
}

void UNPCMenuUI::SetNPCRole(ENPCRole InNPCRole)
{
	ShopButton->SetVisibility(ESlateVisibility::Collapsed);
	ShopLine->SetVisibility(ESlateVisibility::Collapsed);

	UpgradeButton->SetVisibility(ESlateVisibility::Collapsed);
	UpgradeLine->SetVisibility(ESlateVisibility::Collapsed);

	switch (InNPCRole)
	{
		case ENPCRole::Merchant:
			ShopButton->SetVisibility(ESlateVisibility::Visible);
			ShopLine->SetVisibility(ESlateVisibility::Visible);
			break;
		case ENPCRole::Blacksmith:
			UpgradeButton->SetVisibility(ESlateVisibility::Visible);
			UpgradeLine->SetVisibility(ESlateVisibility::Visible);
			break;
		default:
			break;
	}
}