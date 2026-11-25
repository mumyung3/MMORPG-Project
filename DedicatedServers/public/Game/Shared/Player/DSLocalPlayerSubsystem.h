// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/HTTPRequestTypes/HTTPRequestTypes.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "DSLocalPlayerSubsystem.generated.h"

class ILoginManagement;
class ULoginManager;
/**
 *
 */
UCLASS()
class DEDICATEDSERVERS_API UDSLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	void InitializeTokens(const FDSAuthenticationResult& AuthResult, TScriptInterface<ILoginManagement> LoginManagement);
	void SetRefreshTokenTimer();
	void UpdateTokens(const FString& AccessToken, const FString& IdToken);
	FDSAuthenticationResult GetAuthResult() const;

	// ui 띄울 생각이면 띄우기
	FString Username;
	FString Email;

	UPROPERTY()
	TScriptInterface<ILoginManagement> LoginManagementInterface;

private:
	UPROPERTY()
	FDSAuthenticationResult AuthenticationResult;
	
	// 75% of an hour (the expiration time for AccessToken and IdToken)
	float TokenRefreshInterval = 2700.f;
	FTimerHandle RefreshTimer;
	
};
