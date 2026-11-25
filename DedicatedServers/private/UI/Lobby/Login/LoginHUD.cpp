// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/Login/LoginHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Lobby/Login/LoginOverlay.h"
#include "UI/Lobby/MainMenu/MainMenuOverlay.h"

void ALoginHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) && LoginOverlayClass)
	{

		LoginOverlay = CreateWidget<ULoginOverlay>(PlayerController, LoginOverlayClass);
		LoginOverlay->AddToViewport();
	}
}

void ALoginHUD::OnSignIn()
{
	if (IsValid(LoginOverlay))
	{
		LoginOverlay->RemoveFromParent();
	}
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	MainMenuOverlay = CreateWidget<UMainMenuOverlay>(OwningPlayerController, MainMenuOverlayClass);
	if (IsValid(MainMenuOverlay))
	{
		MainMenuOverlay->AddToViewport();
	}
}
void ALoginHUD::OnSignOut()
{
	if (IsValid(MainMenuOverlay))
	{
		MainMenuOverlay->RemoveFromParent();
	}
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	LoginOverlay = CreateWidget<ULoginOverlay>(OwningPlayerController, LoginOverlayClass);
	if (IsValid(LoginOverlay))
	{
		LoginOverlay->AddToViewport();
	}
}