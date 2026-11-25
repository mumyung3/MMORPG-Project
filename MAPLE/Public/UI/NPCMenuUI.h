// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/GameEnums.h"
#include "NPCMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class MAPLE_API UNPCMenuUI : public UUserWidget
{
	GENERATED_BODY()

	TMap<ENPCMenu, TFunction<void()>> ButtonToActionMap; // 버튼마다 실행될 함수를 담고 있는 맵
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NPCNameText;

	// ** Buttons *
	UPROPERTY(meta = (BindWidget))
	class UButton* ShopButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* UpgradeButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* TalkButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	// ** Lines *
	UPROPERTY(meta = (BindWidget))
	class UImage* ShopLine;

	UPROPERTY(meta = (BindWidget))
	class UImage* UpgradeLine;


	// ** Button Functions *
	UFUNCTION()
	void OnShopClicked();

	UFUNCTION()
	void OnUpgradeClicked();

	UFUNCTION()
	void OnTalkClicked();
	
	UFUNCTION()
	void OnExitClicked();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	void SetNPCName(const FText& InNPCName);
	void SetNPCRole(ENPCRole InNPCRole);

	TFunction<void()> OnTalkButton;
	TFunction<void()> OnExitButton;
};
