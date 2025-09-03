// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Lobby/Interfaces/HUDManagement.h"
#include "LoginHUD.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ALoginHUD : public AHUD, public IHUDManagement
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class ULoginOverlay> LoginOverlayClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class UMainMenuOverlay> MainMenuOverlayClass;

	virtual void OnSignIn() override;
	virtual void OnSignOut() override;
	
private:
	UPROPERTY()
	TObjectPtr<ULoginOverlay> LoginOverlay;
	
	UPROPERTY()
	TObjectPtr<UMainMenuOverlay> MainMenuOverlay;
	
protected:
	virtual void BeginPlay() override;
	
};
