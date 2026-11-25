// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGamePlayerBox.generated.h"

struct FDS_PlayerInfoArray;
class ADSGameState;
class UPlayerLabel;
class UScrollBox;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UInGamePlayerBox : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_PlayerInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerLabel> PlayerLabelClass;
protected:
	virtual void NativeOnInitialized() override;
	void UpdatePlayerInfo(ADS_InGameState* InGameState);

	UFUNCTION()
	void OnInGameStateInitialized(ADS_InGameState* InGameState);

	UFUNCTION()
	void CreateAndAddPlayerLabel(const FDS_PlayerInfo& PlayerInfo);

	UFUNCTION()
	void OnPlayerRemoved(const FDS_PlayerInfo& PlayerInfo);

private:

	UPlayerLabel* FindPlayerLabel(const FString& Username);
};
