// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUDBase.generated.h"

class UInGameOverlay;
/**
 *
 */
UCLASS()
class DEDICATEDSERVERS_API AInGameHUDBase : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInGameOverlay> InGameOverlayClass;

	UPROPERTY()
	TObjectPtr<UInGameOverlay> InGameOverlay;
};
