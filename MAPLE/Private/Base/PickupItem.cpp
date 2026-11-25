// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/PickupItem.h"
#include "Character/WarriorHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Object/ItemData.h"
#include "Player/InventoryComp.h"
#include "Util/CustomLog.h"

APickupItem::APickupItem()
{
}

void APickupItem::OnEnterInteract(APawn* Interactor)
{
	PickedUp();
}

void APickupItem::OnInteract()
{
}

void APickupItem::OnExitInteract()
{
}

FText APickupItem::GetInteractDesc()
{
	return InteractDesc;
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	InteractDesc = FText::FromString(TEXT("줍기"));
}

void APickupItem::PickedUp()
{
	AWarriorHeroCharacter* Character = Cast<AWarriorHeroCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (UInventoryComp* Inv = Character->GetInventoryComp())
	{
		FItemData PickedItem;
		PickedItem.ItemID = this->ItemID;
		PickedItem.ItemName = this->ItemName;

		Inv->AddItem(PickedItem);
		LOGC("YOU PICKED UP THE %s", *(Inv->Items.Top().ItemID))
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}