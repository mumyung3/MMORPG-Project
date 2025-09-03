// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/MainMenu/LobbyHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Lobby/MainMenu/MainMenuOverlay.h"
/**
 *@author Daehwan
 *@details Lobby UI Manager
 *@note lobby 허드는 버릴 예정. login hud 에서 인터페이스로 처리
 */
void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) && MainMenuOverlayClass)
	{

		MainMenuOverlay = CreateWidget<UMainMenuOverlay>(PlayerController, MainMenuOverlayClass);
		MainMenuOverlay->AddToViewport();
	}
}