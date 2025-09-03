// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Manger/DragAndDropComponent.h"
#include "Character/WarriorHeroCharacter.h"
#include "Inventory/DS_Item.h"

// Sets default values for this component's properties
UDragAndDropComponent::UDragAndDropComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UDragAndDropComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UDragAndDropComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void UDragAndDropComponent::DragStart(int32 Index, EItemType Type)
{
	switch (Type)
	{
		case EItemType::None:
			UE_LOG(LogTemp, Display, TEXT("Item type is None"));
			break;

		case EItemType::Sword:
			AllItem.Swords[Index].Quantity = 0;
			AllItem.Swords[Index].ItemType = EItemType::None;
			AllItem.Swords[Index].ItemTableRowHandle.RowName = NAME_None;

			break;
		case EItemType::Shield:
			AllItem.Shields[Index].Quantity = 0;
			AllItem.Shields[Index].ItemType = EItemType::None;
			AllItem.Shields[Index].ItemTableRowHandle.RowName = NAME_None;
		break;
		case EItemType::Eatables:
			AllItem.Eatables[Index].Quantity = 0;
			AllItem.Eatables[Index].ItemType = EItemType::None;
			AllItem.Eatables[Index].ItemTableRowHandle.RowName = NAME_None;
		break;
	}

	// UI 갱신
	UpdateInventoryUI();
}
void UDragAndDropComponent::DragEnd(FSlot SourceItem, int32 SourceIndex, int32 DestinationIndex)
{
	switch (SourceItem.ItemType)
	{

		case EItemType::None:
			UE_LOG(LogTemp, Display, TEXT("Item type is None"));
			break;
		case EItemType::Sword:
			AllItem.Swords[SourceIndex] = AllItem.Swords[DestinationIndex];
			AllItem.Swords[DestinationIndex] = SourceItem;
			break;
		case EItemType::Shield:
			AllItem.Shields[SourceIndex] = AllItem.Shields[DestinationIndex];
			AllItem.Shields[DestinationIndex] = SourceItem;
			break;
		case EItemType::Eatables:
			AllItem.Eatables[SourceIndex] = AllItem.Eatables[DestinationIndex];
			AllItem.Eatables[DestinationIndex] = SourceItem;
			break;
	}

	UpdateInventoryUI();
}
void UDragAndDropComponent::Drop(const FSlot& Item)
{
	Server_OnDrop(Item);
}

void UDragAndDropComponent::Server_OnDrop_Implementation(const FSlot& SlotItem){

	FVector Location = FVector(0, 0, 0);
	FRotator Rotation = FRotator(0, 0, 0);

	for (int32 i = 0; i < SlotItem.Quantity ; ++i)
	{
		if (!GetOwner()) return;
		FTransform Transform = Cast<AWarriorHeroCharacter>(GetOwner())->GetActorTransform();
		FVector SpawnLocation = Transform.GetLocation() + (Character->GetActorForwardVector() * 100.0f);
		Transform.SetLocation(SpawnLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = nullptr; // 월드가 부모가 됨
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ADS_Item* Item = GetWorld()->SpawnActor<ADS_Item>(ADS_Item::StaticClass(), Transform,SpawnParams);
		Item->Item.Quantity = 1;
		Item->Item = SlotItem;
		Item->OnRep_InitMesh();

		
	}
	
}
//클라쪽에서 죽었을때 호출한다.
void UDragAndDropComponent::DropAllItem()
{

	for(const FSlot& Slot : AllItem.Swords) {
		if(Slot.Quantity > 0 && Slot.ItemType != EItemType::None && Slot.ItemTableRowHandle.RowName != NAME_None)
		Server_OnDrop_Implementation(Slot);
	}
	for(const FSlot& Slot : AllItem.Shields)
	{
		if(Slot.Quantity > 0 && Slot.ItemType != EItemType::None && Slot.ItemTableRowHandle.RowName != NAME_None)
			Server_OnDrop_Implementation(Slot);
	}
	for(const FSlot& Slot : AllItem.Eatables) { 
		if(Slot.Quantity > 0 && Slot.ItemType != EItemType::None && Slot.ItemTableRowHandle.RowName != NAME_None)
		Server_OnDrop_Implementation(Slot);
	}
	
}