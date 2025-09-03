// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTTP/HTTPRequestManager.h"
#include "Interfaces/IHttpRequest.h"
#include "JoinSessionManager.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UJoinSessionManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	FAPIStatusMessage OnJoinGameSessionMessage;

	void JoinGameSession();
private:

	void FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	FString GetUniquePlayerId() const;
	void HandleGameSessionStatus(const FString& Status, const FString& SessionId);
	void TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionId);

	FTimerHandle CreateSessionTimer;

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> InGameMap;
	
};
