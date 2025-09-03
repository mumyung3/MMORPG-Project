// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WarriorHeroWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"
#include "Kismet/GameplayStatics.h"

UHeroCombatComponent::UHeroCombatComponent()
{
	SetIsReplicatedByDefault(true);
}

AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag_server(FGameplayTag InWeaponTag) const
{
	return Cast<AWarriorHeroWeapon>(GetCharacterCarriedWeaponByTag_server(InWeaponTag));
}

AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCurrentEquippedWeapon_server() const
{
	return Cast<AWarriorHeroWeapon>(GetCharacterCurrentEquippedWeapon_server());
}

float UHeroCombatComponent::GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
	return GetHeroCurrentEquippedWeapon_server()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor)) {
		return;
	}
	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WarriorGameplayTags::Shared_Event_MeleeHit,
		Data
	);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(	//Drop
		GetOwningPawn(),
		WarriorGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
	if (auto HitPawn = Cast<APawn>(HitActor))
	{
		PlayHitSound(HitPawn);
	}
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(	//Drop
		GetOwningPawn(),
		WarriorGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}

void UHeroCombatComponent::PlayHitSound(APawn* HitPawn)
{
	if (GetOwner() && GetOwner()->HasAuthority())
		Multicast_PlayHitSound(HitPawn->GetActorLocation());
}

void UHeroCombatComponent::Multicast_PlayHitSound_Implementation(FVector Location)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			Location,
			1.0f,
			FMath::FRandRange(0.9f, 1.1f),
			0.0f,
			AttenuationSettings
		);
	}
}
