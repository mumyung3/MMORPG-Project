// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/MainMenu/LobbyMenu.h"
#include "DedicatedServers.h"
#include "Game/DS_GameMode.h"
#include "Components/Button.h"

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(Button_Server) && IsValid(Button_Configure) && IsValid(Button_Exit))
	{
		Button_Server->OnClicked.AddDynamic(this, &ULobbyMenu::OnButtonServerClicked);
		Button_Configure->OnClicked.AddDynamic(this, &ULobbyMenu::OnButtonConfigureClicked);
		Button_Exit->OnClicked.AddDynamic(this, &ULobbyMenu::OnButtonExitClicked);
	}
}

void ULobbyMenu::OnButtonServerClicked()
{
	// enum 쓰기
	OnSwitchWidget.Broadcast(ELobbyMenuType::ServerList);
	UE_LOG(DedicatedServers, Log, TEXT("Clicked the Button_Server"));
}

void ULobbyMenu::OnButtonConfigureClicked()
{
	OnSwitchWidget.Broadcast(ELobbyMenuType::Configure);
	UE_LOG(DedicatedServers, Log, TEXT("Clicked the Button_Configure"));
}

void ULobbyMenu::OnButtonExitClicked()
{
	OnSwitchWidget.Broadcast(ELobbyMenuType::Exit);
	UE_LOG(DedicatedServers, Log, TEXT("Clicked the Button_Exit"));
}