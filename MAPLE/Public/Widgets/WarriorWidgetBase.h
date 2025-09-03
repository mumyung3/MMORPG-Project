// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarriorWidgetBase.generated.h"

class UHeroUIComponent;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class MAPLE_API UWarriorWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, meta = (DisplayName = "On Owning Hero UI Component Initialized"))
	void BP_OnOwningHeroUIComponentInitialized(UHeroUIComponent* OwningHeroUIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Enemy UI Component Initialized"))
	void BP_OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningEnemyUIComponent);

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* OwningEnemyActor);
/////////////////////////////////////////////////////////////////////
// 	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
// 	void SetHealthPercentage(float HealthPercentage);
// 
// 	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
// 	void SetManaPercentage(float ManaPercentage);
// 
// 	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
// 	void SetCharacterName(const FText& NewName);
/////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthRegenRate(float HealthRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxStamina(float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentStamina(float CurrentStamina);	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStaminaPercentage(float StaminaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStaminaRegenRate(float StaminaRegenRate);
};
