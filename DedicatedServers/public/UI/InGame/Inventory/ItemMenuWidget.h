// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/Enums/Enums.h"
#include "ItemMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UItemMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Use;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Drop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Close;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> SizeBox_Item;

protected:
	UFUNCTION()
	void OnUseItem();
	virtual void NativeConstruct() override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	
	UFUNCTION()
	void OnClose();

	UFUNCTION()
	void OnDropped();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "info")
	int32 ItemIndex;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "info")
	EItemType ItemType;
};
