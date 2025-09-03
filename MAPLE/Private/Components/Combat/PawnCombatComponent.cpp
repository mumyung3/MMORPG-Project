


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"
#include "WarriorDebugHelper.h"
#include "Net/UnrealNetwork.h"

void UPawnCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPawnCombatComponent, CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A named named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);	//function Emplace is inserting a new key-value
	

	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag_server(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		if (AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			UE_LOG(LogTemp, Warning, TEXT("[PawnCombatComponent.cpp] CharacterCarriedWeaponMap is valid"));
			return *FoundWeapon;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[PawnCombatComponent.cpp] CharacterCarriedWeaponMap is not valid"));
	return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon_server() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PawnCombatComponent.cpp] CurrentEquipedWeaponTag is not valid"));
		return nullptr;
	}
	UE_LOG(LogTemp, Warning, TEXT("[PawnCombatComponent.cpp] CurrentEquipedWeaponTag is valid"));
	return GetCharacterCarriedWeaponByTag_server(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType /*= EToggleDamageType::CurrentEquippedWeapon*/)
{
	UE_LOG(LogTemp, Warning, TEXT("Into ToggleWeaponCollision"));
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon) {
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	}
	else {
		ToggleBodyCollisionBoxCollision(bShouldEnable, ToggleDamageType);
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{

}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{

}

void UPawnCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
	UE_LOG(LogTemp, Warning, TEXT("Into ToggleCurrentEquippedWeaponCollision"));
	if (GetNetMode() == ENetMode::NM_Client)return;

	AWarriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon_server();

	if (!WeaponToToggle) return;
	UE_LOG(LogTemp, Warning, TEXT("[ToggleCurrentEquippedWeaponCollision] WeaponToToggle is valid"));

	if (bShouldEnable)
	{
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(LogTemp, Warning, TEXT("[ToggleCurrentEquippedWeaponCollision] WeaponCollisionBox is Enabled"));
	}
	else
	{
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		OverlappedActors.Empty();
		UE_LOG(LogTemp, Warning, TEXT("[ToggleCurrentEquippedWeaponCollision]OverlappedActors is empty"));
	}
}

void UPawnCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	
}
