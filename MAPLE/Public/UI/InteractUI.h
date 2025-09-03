// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractUI.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API UInteractUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InputKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Description;

public:
	void InputKeySetting(FText InKey);
	void DescriptionSetting(FText Desc);
};
