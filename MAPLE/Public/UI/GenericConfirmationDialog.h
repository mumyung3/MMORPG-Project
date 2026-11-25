// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenericConfirmationDialog.generated.h"

/**
 * 
 */
UCLASS()
class MAPLE_API UGenericConfirmationDialog : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogText;

	UPROPERTY(meta = (BindWidget))
	class UButton* OKButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NOButton;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleOKButtonClicked();
	UFUNCTION()
	void HandleNOButtonClicked();

public:
	TFunction<void()> OnOKClicked; // 확인 버튼이 클릭됐을 때 실행될 함수를 바인딩 (플레이어를 텔레포트시키기)
	
	void SetDialogText(const FText& InMessage);
};
