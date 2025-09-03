// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "WarriorAttributeSet.generated.h"

class IPawnUIInterface;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *
 */
UCLASS()
class MAPLE_API UWarriorAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWarriorAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetPawnCombatComponent(UHeroCombatComponent* InHeroComponent) { CachedHeroCombatComponent = InHeroComponent; };

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_HealthRegenRate)
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, HealthRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_CurrentStamina)
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_StaminaRegenRate)
	FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, StaminaRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Rage", ReplicatedUsing = OnRep_CurrentRage)
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentRage)

	UPROPERTY(BlueprintReadOnly, Category = "Rage", ReplicatedUsing = OnRep_MaxRage)
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxRage)

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DefensePower)
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DamageTaken)
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, DamageTaken)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate);

	UFUNCTION()
	virtual void OnRep_CurrentStamina(const FGameplayAttributeData& OldCurrentStamina);
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	UFUNCTION()
	virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate);

	UFUNCTION()
	virtual void OnRep_CurrentRage(const FGameplayAttributeData& OldCurrentRage);
	UFUNCTION()
	virtual void OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage);
	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);
	UFUNCTION()
	virtual void OnRep_DefensePower(const FGameplayAttributeData& OldDefensePower);
	UFUNCTION()
	virtual void OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken);

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;

	UPROPERTY()
	class UHeroCombatComponent* CachedHeroCombatComponent;
};
