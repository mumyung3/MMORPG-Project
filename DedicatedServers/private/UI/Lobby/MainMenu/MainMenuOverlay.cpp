// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/MainMenu/MainMenuOverlay.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "DedicatedServers/public/HTTP/GameSession/JoinSessionManager.h"
#include "HTTP/ListFleets/ListFleetsManager.h"
#include "UI/Lobby/GameSession/JoinGameWidget.h"
#include "UI/Lobby/ListFleets/ListFleetsBox.h"
#include "UI/Lobby/MainMenu/LobbyMenu.h"
#include "UI/Lobby/MainMenu/Configure/ConfigureBrowser.h"
#include "UI/Lobby/MainMenu/Exit/ExitBrowser.h"
#include "UI/Lobby/MainMenu/Server/ServerBrowser.h"
#include "UI/Lobby/ListFleets/FleetId.h"
void UMainMenuOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(IsValid(WidgetSwitcher));

	OnSwitchToLobbyMenu();
	// LobbyMenu 데리게이트 연결
	check(LobbyMenu);
	LobbyMenu->OnSwitchWidget.AddDynamic(this, &UMainMenuOverlay::SwitchWidget);

	check(IsValid(ExitBrowser));
	ExitBrowser->OnGoBack.AddDynamic(this, &UMainMenuOverlay::OnSwitchToLobbyMenu);

	check(IsValid(ConfigureBrowser));
	ConfigureBrowser->OnGoBack.AddDynamic(this, &UMainMenuOverlay::OnSwitchToLobbyMenu);

	check(IsValid(ServerBrowser));
	ServerBrowser->OnGoBack.AddDynamic(this, &UMainMenuOverlay::OnSwitchToLobbyMenu);

	// ListFleet 연결 델리게이트 아끼기.
	//@todo 마저 구현 해야함. 실제 ListFleet api 사용
	//ListFleetsBox->Button_ListFleets->OnClicked.AddDynamic(this, &UMainMenuOverlay::);
	ServerBrowser->OnSwitchWidget.AddDynamic(this, &UMainMenuOverlay::SwitchWidget);

	//HTTP ListFleets
	check(ListFleetsManagerClass);
	ListFleetsManager = NewObject<UListFleetsManager>(this, ListFleetsManagerClass);
	check(JoinSessionMangerClass);
	JoinSessionManager = NewObject<UJoinSessionManager>(this, JoinSessionMangerClass);

	//유데미 구조와 다르게 짬.
	check(IsValid(ServerBrowser->GetJoinGameWidget()));
	check(IsValid(ServerBrowser->GetJoinGameWidget()->Button_Join));
	ServerBrowser->GetJoinGameWidget()->Button_Join->OnClicked.AddDynamic(this,&UMainMenuOverlay::OnJoinGameSessionClicked);
	
	// 유데미와 구조가 다르게 짬. 
	check(JoinSessionManager);
	check(IsValid(ServerBrowser->GetJoinGameWidget()));
	JoinSessionManager->OnJoinGameSessionMessage.AddDynamic( ServerBrowser->GetJoinGameWidget(), &UJoinGameWidget::UpdateStatusMessage);
	
	check(ListFleetsBox);
	check(ListFleetsBox->Button_ListFleets);
	//버튼 비활 및 기능 호출
	ListFleetsBox->Button_ListFleets->OnClicked.AddDynamic(this, &UMainMenuOverlay::ListFleetsButtonClicked);
	
	//프로필
	check(TextBlock_Email);
	check(TextBlock_Username);
	SetProfile(GetLocalPlayerSubsystem()->Email, GetLocalPlayerSubsystem()->Username);
	
}

void UMainMenuOverlay::ListFleetsButtonClicked()
{
	check(ListFleetsManager);
	//버튼 재활성 및 로그
	ListFleetsManager->OnListFleetsResponseReceived.AddDynamic(this, &UMainMenuOverlay::OnListFleetsResponseReceived);
	ListFleetsManager->ListFleets();
	ListFleetsBox->Button_ListFleets->SetIsEnabled(false);
}

void UMainMenuOverlay::OnListFleetsResponseReceived(const FDSListFleetsResponse& ListFleetsResponse, bool bWasSuccessful)
{
	if (ListFleetsManager->OnListFleetsResponseReceived.IsAlreadyBound(this, &UMainMenuOverlay::OnListFleetsResponseReceived))
	{
		ListFleetsManager->OnListFleetsResponseReceived.RemoveDynamic(this, &UMainMenuOverlay::OnListFleetsResponseReceived);
	}
	ListFleetsBox->ScrollBox_ListFleets->ClearChildren();
	if (bWasSuccessful)
	{
		for (const FString& FleetId : ListFleetsResponse.FleetIds)
		{
			//가비지 컬렉션을 this처리
			check(FleetIdClass);
			UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdClass);
			FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString(FleetId));
			ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
		}
	}
	else
	{
		check(FleetIdClass);
		UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdClass);
		FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString("Something went wrong!"));
		ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
	}
	ListFleetsBox->Button_ListFleets->SetIsEnabled(true);
}

void UMainMenuOverlay::SwitchWidget(ELobbyMenuType Type)
{
	switch (Type)
	{
		case ELobbyMenuType::LobbyMenu:
			OnSwitchToLobbyMenu();
			break;
		case ELobbyMenuType::ServerList:
			OnSwitchToServerBrowser();
			break;
		case ELobbyMenuType::Configure:
			OnSwitchToConfigureBrowser();
			break;
		case ELobbyMenuType::Exit:
			OnSwitchToExitBrowser();
			break;
		case ELobbyMenuType::ListFleets:
			OnSwitchToListFleetsBox();
			break;

		case ELobbyMenuType::None:
			break;
	}
}

void UMainMenuOverlay::OnSwitchToLobbyMenu() 
{
	// 하드 코딩
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenuOverlay::OnSwitchToServerBrowser()
{
	// 하드 코딩
	WidgetSwitcher->SetActiveWidgetIndex(1);
}
void UMainMenuOverlay::OnSwitchToConfigureBrowser() const
{
	// 하드 코딩
	WidgetSwitcher->SetActiveWidgetIndex(2);
}
void UMainMenuOverlay::OnSwitchToExitBrowser() const
{
	// 하드 코딩
	WidgetSwitcher->SetActiveWidgetIndex(3);
}

void UMainMenuOverlay::OnSwitchToListFleetsBox()const
{
	//하드 코딩
	WidgetSwitcher->SetActiveWidgetIndex(4);
}
// JoinGameWidget의 버튼
void UMainMenuOverlay::OnJoinGameSessionClicked()
{
	//이후 응답 실패 받으면 다시 true 성공 시, 트래블.
	JoinSessionManager->JoinGameSession();
	ServerBrowser->GetJoinGameWidget()->Button_Join->SetIsEnabled(false);
	
}

void UMainMenuOverlay::SetProfile(const FString& Email, const FString& Username){

    TextBlock_Email->SetText(FText::FromString(Email));
    TextBlock_Username->SetText(FText::FromString(Username));

}

UDSLocalPlayerSubsystem* UMainMenuOverlay::GetLocalPlayerSubsystem(){
	
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) && IsValid(PlayerController->GetLocalPlayer()))
	{
		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UDSLocalPlayerSubsystem>();
		if (IsValid(LocalPlayerSubsystem))
		{
			return LocalPlayerSubsystem;
		}
	}
	return nullptr;
	
}