// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Shared/Player/DSLocalPlayerSubsystem.h"
#include "HTTP/Login/Interfaces/LoginManagement.h"
#include "HTTP/Login/LoginManager.h"

void UDSLocalPlayerSubsystem::InitializeTokens(const FDSAuthenticationResult& AuthResult, TScriptInterface<ILoginManagement> LoginManagement)
{
	AuthenticationResult = AuthResult;
	LoginManagementInterface = LoginManagement;
	SetRefreshTokenTimer();

}
void UDSLocalPlayerSubsystem::SetRefreshTokenTimer()
{
	UWorld* World = GetWorld();
	if (IsValid(World) && IsValid(LoginManagementInterface.GetObject()))
	{
		FTimerDelegate RefreshDelegate;
		RefreshDelegate.BindLambda([this](){ LoginManagementInterface->RefreshTokens(AuthenticationResult.RefreshToken); });
		World->GetTimerManager().SetTimer(RefreshTimer, RefreshDelegate, TokenRefreshInterval, false);
	}
}

void UDSLocalPlayerSubsystem::UpdateTokens(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}
FDSAuthenticationResult UDSLocalPlayerSubsystem::GetAuthResult() const
{
	return AuthenticationResult;
}
