// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/HTTPRequestTypes/HTTPRequestTypes.h"
#include "HTTP/HTTPRequestManager.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/LoginManagement.h"
#include "LoginManager.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ULoginManager : public UHTTPRequestManager, public ILoginManagement
{
	GENERATED_BODY()

public:

	FAPIStatusMessage SignUpStatusMessageDelegate;

	FAPIStatusMessage ConfirmStatusMessageDelegate;
	FAPIStatusMessage SignInStatusMessageDelegate;

	FOnAPIRequestSucceeded OnSignUpSucceeded;

	FOnAPIRequestSucceeded OnConfirmSucceeded;
	void SignIn(const FString& Username, const FString& Password);
	void SignUp(const FString& Username, const FString& Password, const FString& Email);
	void Confirm(const FString& ConfirmationCode);
	void SignOut(const FString& AccessToken);

	// IPortalManagement
	virtual void RefreshTokens(const FString& RefreshToken) override;


	UFUNCTION()
	void QuitGame();

	FString LastUsername;
	FDSSignUpResponse LastSignUpResponse;
	
private:
	void SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void Confirm_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RefreshTokens_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
