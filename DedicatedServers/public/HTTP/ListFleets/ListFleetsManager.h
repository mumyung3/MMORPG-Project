// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTTP/HTTPRequestManager.h"
#include "Interfaces/IHttpRequest.h"
#include "ListFleetsManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnListFleetsResponseReceived, const FDSListFleetsResponse&, ListFleetsResponse, bool, bWasSuccessful);
/**
 * @detals MainMenuOverlay에서 사용
 */
UCLASS()
class DEDICATEDSERVERS_API UListFleetsManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ListFleets();



	UPROPERTY()
	FOnListFleetsResponseReceived OnListFleetsResponseReceived;

	void ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
