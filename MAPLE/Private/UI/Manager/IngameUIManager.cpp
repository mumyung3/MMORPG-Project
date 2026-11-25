// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Manager/IngameUIManager.h"
#include "Blueprint/UserWidget.h"
#include "UI/GenericConfirmationDialog.h"
#include "UI/NPCDialogUI.h"
#include "UI/SavepointMenuUI.h"
#include "UI/WorldMapUI.h"
#include "DataAssets/NPC/NPCDialogDataAsset.h"
#include "LLMDialogSystem/DialogManagerSubsystem.h"
#include "UI/NPCMenuUI.h"
#include "Util/CustomLog.h"

void UIngameUIManager::RegisterUI(EUIType UIType, const FUIConfig& Config)
{
	UIConfigMap.Add(UIType, Config);
}

void UIngameUIManager::RegisterAllUI(
	TSubclassOf<UUserWidget> InSavepointMenuUIClass,
	TSubclassOf<UUserWidget> InWorldMapUIClass,
	TSubclassOf<UUserWidget> InGenericConfirmationDialogUIClass,
	TSubclassOf<UUserWidget> InNPCDialogUIClass,
	TSubclassOf<UUserWidget> InNPCMenuUIClass,
	TSubclassOf<UUserWidget> InNPCConversationUIClass
)
{
	RegisterUI(EUIType::MainMenu, FUIConfig{
		nullptr,
		[this](UUserWidget* Widget) { LOGC("Main Menu Will be Created") },
		[this](UUserWidget* Widget) { LOGC("Main Menu Will Open") },
		100,
		false,
		false,
		true
	});

	RegisterUI(EUIType::Inventory, FUIConfig{
		nullptr,
		[this](UUserWidget* Widget) { LOGC("Inventory Will be Created") },
		[this](UUserWidget* Widget) { LOGC("Inventory Will Open") },
		200,
		false,
		false,
		true
	});

	RegisterUI(EUIType::SavePointMenu, FUIConfig{
		InSavepointMenuUIClass,
		[this](UUserWidget* Widget)
		{
			if (USavepointMenuUI* SPM = Cast<USavepointMenuUI>(Widget))
			{
				SPM->OnExitButton = [this]() {
					SetInputModeGameOnly();
					SetMenuUIOpened(false);
				};
			}
		},
		nullptr,
		200,
		false,
		false,
		true
	});
	
	RegisterUI(EUIType::WorldMap, FUIConfig{
		InWorldMapUIClass,
		[this](UUserWidget* Widget)
		{
			if (UWorldMapUI* Map = Cast<UWorldMapUI>(Widget))
			{
				Map->InitializeSetting();
				
				Map->OnMapClosedEvent = [this]() {
					SetInputModeGameOnly();
					SetMenuUIOpened(false);
				};

				Map->SetTeleportSavepointID = [this](FName SavepointID) {
					TargetSavepointID = SavepointID;
				};
				
				Map->RequestShowGenericConfirmationDialog = [this](const FText& Text) {
					ShowConfirmationDialog(Text);
				};
			}
		},
		[this](UUserWidget* Widget){},
		300,
		false,
		true,
		true
	});

	RegisterUI(EUIType::ConfirmationDialog, FUIConfig{
		InGenericConfirmationDialogUIClass,
		[](UUserWidget* Widget)
		{
			Widget->AddToViewport(999); // 하드코딩이라 고쳐야 댐.
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		},
		[this](UUserWidget* Widget) {
			if (UGenericConfirmationDialog* GCD = Cast<UGenericConfirmationDialog>(Widget))
			{
				GCD->SetDialogText(CachedDialogText);
				GCD->OnOKClicked = [this](){ // 플레이어가 확인창에서 확인 버튼을 눌렀을 때 실행될 함수를 바인딩
					// !! if 분기로 확인 버튼이 눌렸을 때 실행될 함수 구분 필요 !!
					if (auto Map = Cast<UWorldMapUI>(UIMap[EUIType::WorldMap]))
					{
						Map->HandleEscape(); // 월드맵을 닫고
						RequestTeleportPlayerToSavepoint(TargetSavepointID); // 플레이어를 텔레포트시킴
					}
				};
			}
		},
		999,
		true,
		true,
		true
	});

	RegisterUI(EUIType::NPCDialog, FUIConfig{
		InNPCDialogUIClass,
		[this](UUserWidget* Widget) {
			if (UNPCDialogUI* NPCDialogUI = Cast<UNPCDialogUI>(Widget))
			{
				NPCDialogUI->OnDialogUIClosedEvent = [this]() {
					ShowUI(EUIType::NPCMenu);
				};
			}
		},
		[this](UUserWidget* Widget){},
		200,
		false,
		true,
		false
	});

	RegisterUI(EUIType::NPCMenu, FUIConfig{
		InNPCMenuUIClass,
		[this](UUserWidget* Widget) {
			if (UNPCMenuUI* NPCMenuUI = Cast<UNPCMenuUI>(Widget))
			{
				NPCMenuUI->OnTalkButton = [this]() {
					ShowUI(EUIType::NPCConversation);
				};
				
				NPCMenuUI->OnExitButton = [this]() {
					SetInputModeGameOnly();
					SetMenuUIOpened(false);
				};
			}
		},
		[this](UUserWidget* Widget) {
			
		},
		200,
		false,
		true,
		true
	});

	RegisterUI(EUIType::NPCConversation, FUIConfig{
		InNPCConversationUIClass,
		[this](UUserWidget* Widget) {
			
		},
		[this](UUserWidget* Widget) {
			
		},
		200,
		false,
		false,
		true
	});
}

void UIngameUIManager::CreateUI(EUIType UIType)
{
	if (!UIConfigMap.Contains(UIType)) return;
	const FUIConfig& Config = UIConfigMap[UIType];

	if (!Config.WidgetClass)
	{
		return;
	} // 위젯 클래스 nullptr 예외처리
	
	if (UIMap.Contains(UIType))
		return;
	
	UUserWidget* NewUI = CreateWidget<UUserWidget>(GetWorld(), Config.WidgetClass);
	if (!NewUI) return;

	if (UIMap.Add(UIType, NewUI))

	// 생성 후 콜백(존재할 경우)
	if (Config.OnCreateCallback)
	{
		Config.OnCreateCallback(NewUI);
	}
}

void UIngameUIManager::ShowUI(EUIType UIType)
{
	if (!UIMap.Contains(UIType))
	{
		return;
	}

	if (UUserWidget* UI = UIMap[UIType])
	{
		if (UIConfigMap[UIType].bHideable) // UI SHOW
		{
			UI->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UI->AddToViewport(UIConfigMap[UIType].ZOrder);
		}
		
		if (UIConfigMap[UIType].OnShowCallback) // UI Show CallBack Function IF Needed
		{
			UIConfigMap[UIType].OnShowCallback(UI);
		}

		if (UIConfigMap[UIType].bNeedFocus) // Need Focus
		{
			RequestWidgetFocus(UI, UIConfigMap[UIType].bNeedCursorVisible);
		}
		else
		{
			SetInputModeUIOnly(UIConfigMap[UIType].bNeedCursorVisible);
		}
		
		SetMenuUIOpened(true);
	}
}

void UIngameUIManager::UpdateMapSavepoint(AIngamePlayerState* PS)
{
	if (auto Map = Cast<UWorldMapUI>(UIMap[EUIType::WorldMap]))
	{
		Map->ShowDiscoveredSavepoints(PS);
	}
}

void UIngameUIManager::ShowConfirmationDialog(const FText& Message)
{
	CachedDialogText = Message;
	ShowUI(EUIType::ConfirmationDialog);
}

void UIngameUIManager::ExtractDialogData(UNPCDialogDataAsset* DialogDataAsset, UDialogManagerSubsystem* NPCConvSys)
{
	if (!DialogDataAsset) return;

	if (auto NPCDialogUI = Cast<UNPCDialogUI>(UIMap[EUIType::NPCDialog]))
	{
		NPCDialogUI->SetDialogData(DialogDataAsset->NPCName, DialogDataAsset->DialogueLines);
	}

	if (auto NPCMenuUI = Cast<UNPCMenuUI>(UIMap[EUIType::NPCMenu]))
	{
		NPCMenuUI->SetNPCName(DialogDataAsset->NPCName);
		NPCMenuUI->SetNPCRole(DialogDataAsset->NPCRole);
	}

	FName NPCNameFromText = FName(*DialogDataAsset->NPCName.ToString());
	NPCConvSys->SetNPCName(NPCNameFromText);
}

void UIngameUIManager::SetMenuUIOpened(bool bOpened)
{
	bIsUIOpened = bOpened;
	OnMenuUIOpened.Broadcast(bOpened);
}

bool UIngameUIManager::IsUIOpened()
{
	return bIsUIOpened;
}