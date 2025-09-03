// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/Login/LoginOverlay.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "HTTP/Login/LoginManager.h"
#include "UI/Lobby/Login/SignInPage.h"
#include "UI/Lobby/Login/SignUpPage.h"
#include "UI/Lobby/Login/SuccessConfirmedPage.h"

void ULoginOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	check(LoginManagerClass);
	
	LoginManager = NewObject<ULoginManager>(this, LoginManagerClass);
	
	
	check(SignInPage);	
	SignInPage->Button_SignIn->OnClicked.AddDynamic(this, &ULoginOverlay::SignInButtonClicked);
	SignInPage->Button_SignUp->OnClicked.AddDynamic(this, &ULoginOverlay::ShowSignUpPage);
	SignInPage->Button_Quit->OnClicked.AddDynamic(LoginManager, &ULoginManager::QuitGame);
	LoginManager->SignInStatusMessageDelegate.AddDynamic(SignInPage, &USignInPage::UpdateStatusMessage);	

	check(SignUpPage);	
	SignUpPage->Button_Back->OnClicked.AddDynamic(this, &ULoginOverlay::ShowSignInPage);
	SignUpPage->Button_SignUp->OnClicked.AddDynamic(this, &ULoginOverlay::SignUpButtonClicked);
	check(ConfirmSignUpPage);
	ConfirmSignUpPage->Button_Confirm->OnClicked.AddDynamic(this, &ULoginOverlay::ConfirmButtonClicked);
	//ConfirmSignUpPage->Button_Back->OnClicked.AddDynamic(this, &ULoginOverlay::ShowSignUpPage);
	check(SuccessConfirmedPage);
	SuccessConfirmedPage->Button_Ok->OnClicked.AddDynamic(this, &ULoginOverlay::ShowSignInPage);

	check(LoginManager);
	LoginManager->SignUpStatusMessageDelegate.AddDynamic(SignUpPage,&USignUpPage::UpdateStatusMessage);

	LoginManager->OnConfirmSucceeded.AddDynamic(this, &ULoginOverlay::OnConfirmSucceeded);
	LoginManager->ConfirmStatusMessageDelegate.AddDynamic(ConfirmSignUpPage, &UConfirmSignUpPage::UpdateStatusMessage);
	LoginManager->OnSignUpSucceeded.AddDynamic(this, &ULoginOverlay::OnSignUpSucceeded);
	
}

void ULoginOverlay::ShowSignInPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SignInPage);
}

void ULoginOverlay::ShowSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignUpPage));
	WidgetSwitcher->SetActiveWidget(SignUpPage);
}

void ULoginOverlay::ShowConfirmSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(ConfirmSignUpPage));
	WidgetSwitcher->SetActiveWidget(ConfirmSignUpPage);
}

void ULoginOverlay::ShowSuccessConfirmedPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SuccessConfirmedPage));
	WidgetSwitcher->SetActiveWidget(SuccessConfirmedPage);
}


void ULoginOverlay::SignInButtonClicked()
{
	const FString Username = SignInPage->TextBox_UserName->GetText().ToString();
	const FString Password = SignInPage->TextBox_Password->GetText().ToString();
	LoginManager->SignIn(Username, Password);
}

void ULoginOverlay::SignUpButtonClicked()
{
	const FString Username = SignUpPage->TextBox_UserName->GetText().ToString();
	const FString Password = SignUpPage->TextBox_Password->GetText().ToString();
	const FString Email = SignUpPage->TextBox_Email->GetText().ToString();
	LoginManager->SignUp(Username, Password, Email);
}

void ULoginOverlay::ConfirmButtonClicked()
{
	const FString ConfirmationCode = ConfirmSignUpPage->TextBox_ConfirmationCode->GetText().ToString();
	LoginManager->Confirm(ConfirmationCode);
	ConfirmSignUpPage->Button_Confirm->SetIsEnabled(false);

}


void ULoginOverlay::OnSignUpSucceeded()
{
	SignUpPage->ClearTextBoxes();
	ConfirmSignUpPage->TextBlock_Destination->SetText(FText::FromString(LoginManager->LastSignUpResponse.CodeDeliveryDetails.Destination));
	ShowConfirmSignUpPage();
}

void ULoginOverlay::OnConfirmSucceeded()
{
	ConfirmSignUpPage->ClearTextBoxes();
	ShowSuccessConfirmedPage();
}










