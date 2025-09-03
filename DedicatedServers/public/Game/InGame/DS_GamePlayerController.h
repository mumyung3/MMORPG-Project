// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DS_GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GamePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADS_GamePlayerController();
	UPROPERTY(BlueprintReadOnly)
	FString Username;
	UPROPERTY(BlueprintReadOnly)
	FString PlayerSessionId;
};
