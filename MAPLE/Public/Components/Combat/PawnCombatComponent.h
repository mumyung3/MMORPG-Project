// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AWarriorWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};
/**
 *
 */
UCLASS()
class MAPLE_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);		//Add New WeaponMap(WeaponTag, WeaponBase)

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")	//character has GameplayTag(Weapon)?
		AWarriorWeaponBase* GetCharacterCarriedWeaponByTag_server(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat", Replicated)	//CurrentEquipedWeaponTag
		FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")	//Get Current EquipWeapon
		AWarriorWeaponBase* GetCharacterCurrentEquippedWeapon_server() const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat"/*, Replicated*/)
	TArray<AActor*> OverlappedActors;

	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat")
	TMap<FGameplayTag, AWarriorWeaponBase*> CharacterCarriedWeaponMap; //private before
private:
};
