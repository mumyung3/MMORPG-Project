// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Lobby/MainMenu/Server/ServerBrowser.h"
#include "JoinGameWidget.generated.h"

/**
 *
 */
UCLASS()
class DEDICATEDSERVERS_API UJoinGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Join;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_StatusMessage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_GoBack;

	FOnGoBack OnGoBack;

protected:
	//UFUNCTION()
	//void OnJoinGameSessionClicked();

	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void OnBackServerBrowser();

public:
	UFUNCTION()
	void UpdateStatusMessage(const FString& StatusMessage, bool bResetJoinGameButton);
};
