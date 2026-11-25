// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DedicatedServers/public/DataTypes/Enums/Enums.h"
#include "LobbyMenu.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchWidget, ELobbyMenuType, Type);
/**
 *
 *
 **/

UCLASS()
class DEDICATEDSERVERS_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Server;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Configure;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Exit;

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "LobbyMenu")
	FOnSwitchWidget OnSwitchWidget;

private:
	UFUNCTION()
	void OnButtonServerClicked();
	UFUNCTION()
	void OnButtonConfigureClicked();
	UFUNCTION()
	void OnButtonExitClicked();
};
