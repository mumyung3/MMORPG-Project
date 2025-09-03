// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	void ShowMouseCursorAndSetInputMode();

	protected:
	virtual void BeginPlay() override;
public:
	
	void HideMouseCursorAndSetInputMode();
};
