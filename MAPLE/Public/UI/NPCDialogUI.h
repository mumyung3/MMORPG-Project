// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/EscClosableUI.h"
#include "NPCDialogUI.generated.h"

/**
 * 
 */
UCLASS()
class MAPLE_API UNPCDialogUI : public UUserWidget, public IEscClosableUI
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NPCNameText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NPCDialogText;

	TArray<FText> DialogueLines;
	int32 CurrentDialogLineIndex = 0;
	bool bIsDialogLineEnded = false;

protected:
	virtual void NativeConstruct() override;
	
	void NextButtonPressed();
	void UpdateDialogText();

public:
	void SetDialogData(const FText& InNPCName, const TArray<FText>& InNPCDialogLines);

	virtual void HandleEscape() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	TFunction<void()> OnDialogUIClosedEvent; // 대화창을 닫을 때 실행되는 콜백
};
