// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavepointMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class MAPLE_API USavepointMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
	TMap<class UButton*, TFunction<void()>> ButtonToActionMap; // 버튼마다 실행될 함수를 담고 있는 맵

	UPROPERTY(meta = (BindWidget))
	class UButton* LvUpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PotionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnLvUpClicked();

	UFUNCTION()
	void OnPotionClicked();

	UFUNCTION()
	void OnSaveClicked();

	UFUNCTION()
	void OnExitClicked();

public:
	TFunction<void()> OnExitButton;
};