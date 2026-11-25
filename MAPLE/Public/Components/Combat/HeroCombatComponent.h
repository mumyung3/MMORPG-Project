// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class AWarriorHeroWeapon;
/**
 *
 */

UCLASS()
class MAPLE_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UHeroCombatComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* GetHeroCarriedWeaponByTag_server(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* GetHeroCurrentEquippedWeapon_server() const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const;

	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;

	// ** Hit Sound *
	void PlayHitSound(APawn* HitPawn);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayHitSound(FVector Location);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundAttenuation* AttenuationSettings;
};
