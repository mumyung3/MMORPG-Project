// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes/HTTPRequestTypes/HTTPRequestTypes.h"
#include "Game/Shared/Player/DSLocalPlayerSubsystem.h"
#include "UI/Lobby/MainMenu/LobbyMenu.h"
#include "MainMenuOverlay.generated.h"

class UTextBlock;
class UJoinSessionManager;
class UFleetId;
class UListFleetsManager;
struct FDSListFleetsResponse;
/**
 *
 */
UCLASS()
class DEDICATEDSERVERS_API UMainMenuOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	//안뜨는 문제 해결 계속 코드 수정해봐야하는듯
	UPROPERTY(EditAnywhere)
	TSubclassOf<UListFleetsManager> ListFleetsManagerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UJoinSessionManager> JoinSessionMangerClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFleetId> FleetIdClass;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class ULobbyMenu> LobbyMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UServerBrowser> ServerBrowser;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UConfigureBrowser> ConfigureBrowser;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UExitBrowser> ExitBrowser;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListFleetsBox> ListFleetsBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Username;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Email;

	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ListFleetsButtonClicked();

	UFUNCTION()
	void OnListFleetsResponseReceived(const FDSListFleetsResponse& ListFleetsResponse, bool bWasSuccessful);

private:
	UFUNCTION()
	void OnSwitchToLobbyMenu();

	UFUNCTION()
	void OnSwitchToServerBrowser();

	UFUNCTION()
	void OnSwitchToConfigureBrowser() const;

	UFUNCTION()
	void OnSwitchToExitBrowser() const;

	UFUNCTION()
	void OnSwitchToListFleetsBox() const;
	UFUNCTION()
	void OnJoinGameSessionClicked();

	UFUNCTION()
	void SwitchWidget(ELobbyMenuType Type);

	// HTTP 모아두겠음.
private:
	UPROPERTY()
	TObjectPtr<UListFleetsManager> ListFleetsManager;

	UPROPERTY()
	TObjectPtr<UJoinSessionManager> JoinSessionManager;

	void SetProfile(const FString& Email, const FString& Username);
	UDSLocalPlayerSubsystem* GetLocalPlayerSubsystem();
};
