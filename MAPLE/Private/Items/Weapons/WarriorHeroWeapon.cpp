// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WarriorHeroWeapon.h"
#include "Net/UnrealNetwork.h"

void AWarriorHeroWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWarriorHeroWeapon, HeroWeaponData);
}

void AWarriorHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	UE_LOG(LogTemp, Warning, TEXT("AssignGrantedAbilitySpecHandles"));
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AWarriorHeroWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
