// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayEffectTypes.h"
#include "IngamePlayerState.generated.h"

class UWarriorAbilitySystemComponent;
class UWarriorAttributeSet;
/**
 * 
 */
UCLASS()
class MAPLE_API AIngamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AIngamePlayerState();

	UFUNCTION()
	void OnRep_DiscoveredSavepoints();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // Replicate 관련 함수
	
	UPROPERTY(ReplicatedUsing=OnRep_DiscoveredSavepoints)
	TArray<FName> DiscoveredSavepoints; // 플레이어가 발견한 세이브포인트를 저장할 배열

	void AddDiscoveredSavepoint(FName SavepointID); // 세이브포인트를 받아와 배열에 저장
	
	//** Getter Function *
	const TArray<FName>& GetDiscoveredSavepoints();

	UWarriorAbilitySystemComponent* GetAbilitySystemComponent() const ;
	UWarriorAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetHealthRegenRate() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina() const;
	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const;
	UFUNCTION(BlueprintCallable)
	float GetStaminaRegenRate() const;

protected:
	UPROPERTY()
	UWarriorAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UWarriorAttributeSet* AttributeSetBase;

	FDelegateHandle CurrentHealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;

	FDelegateHandle CurrentStaminaChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;
	FDelegateHandle StaminaRegenRateChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void CurrentHealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void CurrentStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaRegenRateChanged(const FOnAttributeChangeData& Data);
};
