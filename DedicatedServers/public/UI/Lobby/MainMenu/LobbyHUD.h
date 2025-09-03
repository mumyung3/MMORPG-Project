// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 *
 */
UCLASS()
class DEDICATEDSERVERS_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class UMainMenuOverlay> MainMenuOverlayClass;

private:
	UPROPERTY()
	TObjectPtr<class UMainMenuOverlay> MainMenuOverlay;
};
