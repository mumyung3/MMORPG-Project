// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DSGameState.generated.h"

class ADS_InGameState;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInGameStateInitialized, ADS_InGameState*, InGameState);
/**
 *
 */
UCLASS()
class DEDICATEDSERVERS_API ADSGameState : public AGameState
{
	GENERATED_BODY()
public:
	ADSGameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_InGameState)
	TObjectPtr<ADS_InGameState> InGameState;

	UPROPERTY(BlueprintAssignable)
	FOnInGameStateInitialized OnInGameStateInitialized;

protected:
	virtual void BeginPlay() override;
	void CreateInGameState();
private:

	UFUNCTION()
	void OnRep_InGameState();
};