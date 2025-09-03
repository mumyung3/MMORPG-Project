// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/IngamePlayerState.h"

#include "Net/UnrealNetwork.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "Character/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"
#include "Widgets/WarriorWidgetBase.h"

 AIngamePlayerState::AIngamePlayerState()
{
 	AbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
 	AbilitySystemComponent->SetIsReplicated(true);
 	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

 	AttributeSetBase = CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("AttributeSetBase"));

 	NetUpdateFrequency = 100.0f;
}

void AIngamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AIngamePlayerState, DiscoveredSavepoints);
}

void AIngamePlayerState::OnRep_DiscoveredSavepoints()
{
	// 배열에 변경이 생길 시 실행됨.
	if (AWarriorHeroController* PC = GetOwner<AWarriorHeroController>())
	{
		PC->UpdateSavepointMarkerVisibility();
	}
}

void AIngamePlayerState::AddDiscoveredSavepoint(FName SavepointID)
{
	if (!DiscoveredSavepoints.Contains(SavepointID))
	{
		DiscoveredSavepoints.Add(SavepointID);
	}
}

const TArray<FName>& AIngamePlayerState::GetDiscoveredSavepoints()
{
	return DiscoveredSavepoints;
}

UWarriorAbilitySystemComponent* AIngamePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UWarriorAttributeSet* AIngamePlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

float AIngamePlayerState::GetCurrentHealth() const
{
	return AttributeSetBase->GetCurrentHealth();
}

float AIngamePlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AIngamePlayerState::GetHealthRegenRate() const
{
	return AttributeSetBase->GetHealthRegenRate();
}

float AIngamePlayerState::GetCurrentStamina() const
{
	return AttributeSetBase->GetCurrentStamina();
}

float AIngamePlayerState::GetMaxStamina() const
{
	return AttributeSetBase->GetMaxStamina();
}

float AIngamePlayerState::GetStaminaRegenRate() const
{
	return AttributeSetBase->GetStaminaRegenRate();
}

void AIngamePlayerState::BeginPlay()
{
	CurrentHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCurrentHealthAttribute()).AddUObject(this, &AIngamePlayerState::CurrentHealthChanged);
	MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AIngamePlayerState::MaxHealthChanged);
	HealthRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthRegenRateAttribute()).AddUObject(this, &AIngamePlayerState::HealthRegenRateChanged);
	CurrentStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCurrentStaminaAttribute()).AddUObject(this, &AIngamePlayerState::CurrentStaminaChanged);
	MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxStaminaAttribute()).AddUObject(this, &AIngamePlayerState::MaxStaminaChanged);
	StaminaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaRegenRateAttribute()).AddUObject(this, &AIngamePlayerState::StaminaRegenRateChanged);
}

void AIngamePlayerState::CurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	
}

void AIngamePlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{

}

void AIngamePlayerState::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{

}

void AIngamePlayerState::CurrentStaminaChanged(const FOnAttributeChangeData& Data)
{

}

void AIngamePlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{

}

void AIngamePlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{

}
