// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Lobby/LobbyPlayerController.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShowMouseCursorAndSetInputMode();
}

void ALobbyPlayerController::HideMouseCursorAndSetInputMode()
{
	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
void ALobbyPlayerController::ShowMouseCursorAndSetInputMode()
{
	// 마우스 커서 보이기
	bShowMouseCursor = true;

	// 입력 모드 설정 (UI 전용 모드)
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);


	
}
