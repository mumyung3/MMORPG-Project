// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Lobby/MainMenu/LobbyMenu.h"
#include "ServerBrowser.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoBack);

/**
 *
 */
UCLASS()
class DEDICATEDSERVERS_API UServerBrowser : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Server1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Server2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Join;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_GoBack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UJoinGameWidget> JoinGameWidget;

	UFUNCTION()
	void GoBackWidget();

	UFUNCTION()
	void SwitchToServerBrowser();

	UFUNCTION()
	void SwitchToJoinGame();
	/**
	 * @todo 실제 서버 입장 구현 해야함.
	 * @details listfleets 확인용 코드 해야함.
	*/
	UFUNCTION()
	void JoinServer();
	
	protected:
	virtual void NativeConstruct() override;

public:
	FOnGoBack OnGoBack;
	FOnSwitchWidget OnSwitchWidget;

	TObjectPtr<UJoinGameWidget> GetJoinGameWidget() const;
};
