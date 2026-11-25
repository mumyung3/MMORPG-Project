// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGame/InGameHUDBase.h"

#include "Blueprint/UserWidget.h"
#include "UI/InGame/InGameOverlay.h"
void AInGameHUDBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController) && InGameOverlayClass)
	{

		InGameOverlay = CreateWidget<UInGameOverlay>(PlayerController, InGameOverlayClass);
		InGameOverlay->AddToViewport();
	}
	
}