// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameOverlay.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UInGameOverlay : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UInventoryWidget> Inventory;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPickUpWidget> PickUp;

	void SetMoney(int32 Money);


};
