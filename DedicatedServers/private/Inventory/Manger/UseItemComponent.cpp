// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Manger/UseItemComponent.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "Inventory/DS_Item.h"
#include "Items/Weapons/WarriorHeroWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Util/CustomLog.h"




// Sets default values for this component's properties
UUseItemComponent::UUseItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

// ...
}

// Called when the game starts
void UUseItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UUseItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
// Use == 먹기와 장착 부분
void UUseItemComponent::UseItem(int32 Index, EItemType Type)
{
	// 타입 확인
	switch (Type)
	{
		case EItemType::None:
			UE_LOG(LogTemp, Display, TEXT("Item type is None"));
			break;
		
		case EItemType::Sword:
			UseSword(Index , Type);
			break;

		case EItemType::Shield:
			UseShield(Index , Type);
			break;
		
		case EItemType::Eatables:
			UseEatable(Index ,Type);
			break;
	}

	// 수량 제거 후
	UpdateInventoryUI();
}

void UUseItemComponent::UseSword(int32 Index,EItemType Type)
{
	FSlot Sword = AllItem.Swords[Index];
	FItem* Item = Sword.ItemTableRowHandle.DataTable->FindRow<FItem>(Sword.ItemTableRowHandle.RowName, TEXT(""));
	auto* Weapon = GetSwordActor();
	
	if (!Weapon)
	{
		return;
	}
	Server_OnWeaponEquipped(*Item);
}

void UUseItemComponent::UseShield(int32 Index, EItemType Type)
{
	FSlot  Sword = AllItem.Shields[Index];
	FItem* Item = Sword.ItemTableRowHandle.DataTable->FindRow<FItem>(Sword.ItemTableRowHandle.RowName, TEXT(""));
	auto*  Shield = GetSwordActor();

	if (!Shield)
	{
		return;
	}
	
	Server_OnShieldEquipped(*Item);
}

void UUseItemComponent::UseEatable(int32 Index,EItemType Type)
{
	Eatable = AllItem.Eatables[Index];
	AllItem.Eatables[Index].Quantity--;
	//	물론 서버에서
	ServerRequestHeal(Eatable);
}
// 다 빼버리기
void UUseItemComponent::OnDrop(int32 Index, EItemType Type)
{
	switch (Type)
	{
		case EItemType::None:
			UE_LOG(LogTemp, Display, TEXT("Item type is None"));
			break;

		case EItemType::Sword:
			Server_OnDrop(AllItem.Swords[Index]);
			AllItem.Swords[Index].Quantity = 0;
			break;

		case EItemType::Shield:
			Server_OnDrop(AllItem.Shields[Index]);
			AllItem.Shields[Index].Quantity = 0;
			break;

		case EItemType::Eatables:
			Server_OnDrop(AllItem.Eatables[Index]);
			AllItem.Eatables[Index].Quantity = 0;
			break;
	}

	// 수량 제거 후
	UpdateInventoryUI();
}

// 어빌리티 사용
void UUseItemComponent::ServerRequestHeal_Implementation(const FSlot& SlotItem){
	UE_LOG(LogTemp, Warning, TEXT("ServerRequestHeal called!"));
	//아이템 파워 정보 전달
	Eatable = SlotItem;

	if (Eatable.ItemTableRowHandle.DataTable && Eatable.ItemTableRowHandle.RowName != NAME_None)
	{
		if (FItem* Item = Eatable.ItemTableRowHandle.DataTable->FindRow<FItem>(Eatable.ItemTableRowHandle.RowName, TEXT("")))
		{
			Power = Item->Power;
		}
	}

	if (auto* HeroCharacter = Cast<AWarriorHeroCharacter>(GetOwner()))
	{
    
		FGameplayTag HealInputTag = FGameplayTag::RequestGameplayTag(FName("InputTag.Heal"));
		HeroCharacter->Input_AbilityInputPressed(HealInputTag);
	}
}

AWarriorHeroWeapon* UUseItemComponent::GetSwordActor()
{
	return WarriorHeroWeapon;
}
void UUseItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UDragAndDropComponent::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUseItemComponent, WarriorHeroWeapon);
}

void UUseItemComponent::OnRep_InitWeapon()
{
	if (!WarriorHeroWeapon)
		return;
	LOGA("초기화됨")
}
void UUseItemComponent::Server_OnWeaponEquipped_Implementation(const FItem& Item)
{
	Multicast_OnWeaponEquipped(Item);
}
void UUseItemComponent::Multicast_OnWeaponEquipped_Implementation(const FItem& Item){

	auto* Weapon = GetSwordActor();
	LOGA("WEAPON 이전")
	if (!Weapon)
	{
		return;
	}
	LOGA("WEAPON 성공")

	Weapon->OnWeaponEquipped(Item);

}

void UUseItemComponent::Server_OnShieldEquipped_Implementation(const FItem& Item)
{
	Multicast_OnShieldEquipped(Item);
}
void UUseItemComponent::Multicast_OnShieldEquipped_Implementation(const FItem& Item){
	Character->ShieldMeshComp->SetStaticMesh(Item.Mesh);

}

void UUseItemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);

	if (!GetSwordActor()) return;

	Server_DestroyWeapon();
}

void UUseItemComponent::Server_DestroyWeapon_Implementation(){
	if (!GetSwordActor()) return;
	GetSwordActor()->Destroy();
	
}