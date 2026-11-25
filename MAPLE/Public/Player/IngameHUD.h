// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/InGame/InGameHUDBase.h"
#include "IngameHUD.generated.h"

class UMain;
/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMainUIReady, UMain*, MainUI);

UCLASS()
class MAPLE_API AIngameHUD : public AInGameHUDBase
{
	GENERATED_BODY()
	
	UPROPERTY()
	class UMain* MainUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMain> MainUIClass;

	APlayerController* OwnerPC = nullptr;

protected:
	virtual void BeginPlay() override;

public:
	FOnMainUIReady OnMainUIReady;
	

	UMain* GetMainUI() { return MainUI; }
};
