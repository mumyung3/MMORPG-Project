// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "WarriorHeroWeapon.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", Replicated)
	FWarriorHeroWeaponData HeroWeaponData;

	/*	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;*/

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;


public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponEquipped(const FItem& Item);
};
