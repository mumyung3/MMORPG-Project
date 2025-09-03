// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConfirmSignUpPage.h"
#include "Blueprint/UserWidget.h"
#include "LoginOverlay.generated.h"

class USignUpPage;
class USuccessConfirmedPage;
class UWidgetSwitcher;
class UButton;
class USignInPage;
class UJoinGameWidget;
class ULoginManager;
class UConfirmSignUpPage;
/**
 *
 */
UCLASS()
class DEDICATEDSERVERS_API ULoginOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoginManager> LoginManagerClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignInPage> SignInPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignUpPage> SignUpPage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UConfirmSignUpPage> ConfirmSignUpPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USuccessConfirmedPage> SuccessConfirmedPage;

	UPROPERTY()
	TObjectPtr<ULoginManager> LoginManager;
	
	UFUNCTION()
	void ShowSignInPage();

	UFUNCTION()
	void ShowSignUpPage();

	UFUNCTION()
	void ShowConfirmSignUpPage();

	UFUNCTION()
	void ShowSuccessConfirmedPage();
	UFUNCTION()
	void SignInButtonClicked();
	UFUNCTION()
	void SignUpButtonClicked();
	UFUNCTION()
	void ConfirmButtonClicked();
	UFUNCTION()
	void OnSignUpSucceeded();
	UFUNCTION()
	void OnConfirmSucceeded();
};
