// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/MainMenu/Server/ServerBrowser.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Lobby/GameSession/JoinGameWidget.h"

void UServerBrowser::NativeConstruct()
{
	Super::NativeConstruct();
	check(IsValid(Button_GoBack));
	Button_GoBack->OnClicked.AddDynamic(this, &UServerBrowser::GoBackWidget);
	Button_Server1->OnClicked.AddDynamic(this, &UServerBrowser::JoinServer);
	SwitchToServerBrowser();
	JoinGameWidget->OnGoBack.AddDynamic(this, &UServerBrowser::SwitchToJoinGame);

	check(IsValid(JoinGameWidget));
	JoinGameWidget->OnGoBack.AddDynamic(this, &UServerBrowser::SwitchToServerBrowser);
}
TObjectPtr<UJoinGameWidget> UServerBrowser::GetJoinGameWidget() const
{
	return JoinGameWidget;
}

void UServerBrowser::GoBackWidget()
{

	OnGoBack.Broadcast();
}
void UServerBrowser::JoinServer()
{
	// ListFleets 보여주기
	//OnSwitchWidget.Broadcast(ELobbyMenuType::ListFleets);
	
	SwitchToJoinGame();
}

void UServerBrowser::SwitchToServerBrowser()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}
void UServerBrowser::SwitchToJoinGame(){
	WidgetSwitcher->SetActiveWidgetIndex(1);

}
