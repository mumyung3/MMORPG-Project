// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WarriorBaseCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "MotionWarpingComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "Player/IngamePlayerState.h"

// Sets default values
AWarriorBaseCharacter::AWarriorBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false; //ignore decals

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* AWarriorBaseCharacter::GetAbilitySystemComponent() const
{
	return GetWarriorAbilitySystemComponent();
}

UPawnCombatComponent* AWarriorBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AWarriorBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

float AWarriorBaseCharacter::GetCurrentHealth() const
{
	if (WarriorAttributeSet.IsValid())
	{
		return WarriorAttributeSet->GetCurrentHealth();
	}
	return 0.0f;
}

float AWarriorBaseCharacter::GetMaxHealth() const
{
	if (WarriorAttributeSet.IsValid())
	{
		return WarriorAttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AWarriorBaseCharacter::GetCurrentStamina() const
{
	if (WarriorAttributeSet.IsValid())
	{
		return WarriorAttributeSet->GetCurrentStamina();
	}
	return 0.0f;
}

float AWarriorBaseCharacter::GetMaxStamina() const
{
	if (WarriorAttributeSet.IsValid())
	{
		return WarriorAttributeSet->GetMaxStamina();
	}
	return 0.0f;
}

void AWarriorBaseCharacter::PossessedBy(AController* NewController) {		//Function in the APawn 
	Super::PossessedBy(NewController);
}

void AWarriorBaseCharacter::SetHealth(float Health)
{
	if (WarriorAttributeSet.IsValid())
	{
		WarriorAttributeSet->SetCurrentHealth(Health);
	}
}

void AWarriorBaseCharacter::SetStamina(float Stamina)
{
	if (WarriorAttributeSet.IsValid())
	{
		WarriorAttributeSet->SetCurrentStamina(Stamina);
	}
}