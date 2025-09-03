// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Lobby/MainMenu/Server/ServerBrowser.h"
#include "ConfigureBrowser.generated.h"


/**
 *@todo 임시창
 */
UCLASS()
class DEDICATEDSERVERS_API UConfigureBrowser : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_GoBack;

	UFUNCTION()
	void GoBackWidget();
	
	protected:
	virtual void NativeConstruct() override;

public:
	FOnGoBack OnGoBack;
};
