// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "Net/UnrealNetwork.h"
#include "Util/CustomLog.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentStamina(1.f);
	InitMaxStamina(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid()) {
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
		// == CachedPawnUIInterface = Cast<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel())

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

	checkf(PawnUIComponent, TEXT("Couldn't extra a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute()) {
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
	}
	if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		const float NewCurrentStamina = FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStamina());

		SetCurrentStamina(NewCurrentStamina);
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			HeroUIComponent->OnCurrentStaminaChanged.Broadcast(GetCurrentStamina() / GetMaxStamina());
		}
	}
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute()) {
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

		SetCurrentRage(NewCurrentRage);

		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent()) {
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage()/GetMaxRage());
		}
	}
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute()) {
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		const FString DebugString = FString::Printf(
			TEXT("Old Health: %f, Damage Done: %f, NewCurrentHealth: %f"),
			OldHealth,
			DamageDone,
			NewCurrentHealth
		);

		//Debug::print(DebugString, FColor::Green);

		//Notify the UI
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());

		if (GetCurrentHealth() <= 0.f) {
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Dead);
		}
		UE_LOG(LogTemp, Warning, TEXT("Player Has Shared_Status_Dead : %d"), UWarriorFunctionLibrary::NativeDoesActorHaveTag(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Dead));

		// DamageTaken 리셋
		SetDamageTaken(0.f);
		
		// Sound
		AActor* RawActor = GetOwningAbilitySystemComponent() ? GetOwningAbilitySystemComponent()->GetAvatarActor() : nullptr;
		APawn* Target = Cast<APawn>(RawActor);
		if (CachedHeroCombatComponent)
		{
			CachedHeroCombatComponent->PlayHitSound(Target);
		}
	}
}

void UWarriorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, CurrentStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, StaminaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, CurrentRage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, MaxRage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, DefensePower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, DamageTaken, COND_None, REPNOTIFY_Always);
}

void UWarriorAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, CurrentHealth, OldCurrentHealth);
	
	AActor* AvatarActor = GetOwningAbilitySystemComponent() ? GetOwningAbilitySystemComponent()->GetAvatarActor() : nullptr;
	if (!AvatarActor) {return;}
	
	IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(AvatarActor);
	if (!PawnUIInterface) {return;}
	
	UPawnUIComponent* PawnUIComponent = PawnUIInterface->GetPawnUIComponent();
	if (!PawnUIComponent) {return;}
	
	PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	
	UE_LOG(LogTemp, Warning, TEXT("WAttributeSet.cpp / CurrentHealth = %f"), OldCurrentHealth.GetCurrentValue());
}

void UWarriorAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, MaxHealth, OldMaxHealth);
}

void UWarriorAttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, HealthRegenRate, OldHealthRegenRate);
}

void UWarriorAttributeSet::OnRep_CurrentStamina(const FGameplayAttributeData& OldCurrentStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, CurrentStamina, OldCurrentStamina);

	AActor* AvatarActor = GetOwningAbilitySystemComponent() ? GetOwningAbilitySystemComponent()->GetAvatarActor() : nullptr;
	if (!AvatarActor) { return; }

	IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(AvatarActor);
	if (!PawnUIInterface) { return; }

	UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent();
	if (!HeroUIComponent) { return; }

	HeroUIComponent->OnCurrentStaminaChanged.Broadcast(GetCurrentStamina() / GetMaxStamina());

	UE_LOG(LogTemp, Warning, TEXT("CurrentStamina = %f"), OldCurrentStamina.GetCurrentValue());
}

void UWarriorAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, MaxStamina, OldMaxStamina);
}

void UWarriorAttributeSet::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, StaminaRegenRate, OldStaminaRegenRate);
}

void UWarriorAttributeSet::OnRep_CurrentRage(const FGameplayAttributeData& OldCurrentRage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, CurrentRage, OldCurrentRage);
}

void UWarriorAttributeSet::OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, MaxRage, OldMaxRage);
}

void UWarriorAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, AttackPower, OldAttackPower);
}

void UWarriorAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldDefensePower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, DefensePower, OldDefensePower);
}

void UWarriorAttributeSet::OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, DamageTaken, OldDamageTaken);
}
