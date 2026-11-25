// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Lobby/MainMenu/Server/ServerBrowser.h"
#include "ExitBrowser.generated.h"

class UDSLocalPlayerSubsystem;
/**
 * @todo 나가기 전에, 로그아웃 기능 연결하기.
 */
UCLASS()
class DEDICATEDSERVERS_API UExitBrowser : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Exit;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_GoBack;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnExit();
	UFUNCTION()
	void OnGoBackWidget();

public:
	//UPROPERTY()
	FOnGoBack OnGoBack;

	UDSLocalPlayerSubsystem* GetLocalPlayerSubsystem();
};
