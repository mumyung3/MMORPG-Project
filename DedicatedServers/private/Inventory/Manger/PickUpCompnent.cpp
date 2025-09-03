// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Manger/PickUpCompnent.h"

#include "EnhancedInputComponent.h"
#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"
#include "Character/WarriorEnemyCharacter.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controllers/WarriorHeroController.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "Inventory/DS_Item.h"
#include "Inventory/DS_Money.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/IngameHUD.h"
#include "UI/InGame/InGameOverlay.h"
#include "UI/InGame/Inventory/InventoryWidget.h"
#include "UI/InGame/Inventory/ItemInfoWidget.h"
#include "UI/InGame/Inventory/PickUpWidget.h"
#include "Util/CustomLog.h"

// Sets default values for this component's properties
UPickUpCompnent::UPickUpCompnent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UPickUpCompnent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);

	if (!GetOwner()) return;
	Character = Cast<AWarriorHeroCharacter>(GetOwner());

	//server code
	if (GetOwner()->HasAuthority()){
		check(Character);
		//ABOUT MONEY 
		Character->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UPickUpCompnent::OnComponentBeginOverlap);
		//ABOUT ITEMS
		Character->CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &UPickUpCompnent::OnCapsuleCompBeginOverlap);
		Character->CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &UPickUpCompnent::OnCapsuleCompEndOverlap);

	}
	
	//client code and Only Player
	if (!GetOwner()->HasAuthority() && Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		UpdateInventoryUI();
		PlayBackgroundMusic();

	}
	


}	

// Called every frame
void UPickUpCompnent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

//점프 기능도 그냥여기다가
void UPickUpCompnent::SetupPlayerInputComponent(UEnhancedInputComponent* Cast)
{
	Cast->BindAction(IA_KeyI, ETriggerEvent::Started, this, &UPickUpCompnent::IA_ToggleInventory);
	Cast->BindAction(IA_KeyE, ETriggerEvent::Started, this, &UPickUpCompnent::IA_PickUpItem);
	Cast->BindAction(IA_KeySpace, ETriggerEvent::Started, this, &UPickUpCompnent::OnJump);
	Cast->BindAction(IA_KeyESC, ETriggerEvent::Started, this, &UPickUpCompnent::IA_QuitGame);
	Cast->BindAction(IA_KeyRespawn, ETriggerEvent::Started, this, &UPickUpCompnent::ForceRespawn);



}
void UPickUpCompnent::OnComponentBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (ADS_Money* MoneyItem = Cast<ADS_Money>(OtherActor); IsValid(MoneyItem))
	{
		FMoney* Item = MoneyItem->MoneyTableRowHandle.GetRow<FMoney>(MoneyItem->MoneyTableRowHandle.RowName.ToString());
		Money += Item->Amount;
		Net_Multicast_DropSound();
		MoneyItem->Destroy();
		// LOGA("Money: %d", Money);
		// 돈 먹는 소리 추가
	}
	
}

void UPickUpCompnent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPickUpCompnent, Money);
}
void UPickUpCompnent::OnRep_Money()
{
	// Client code && Only Player
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			AHUD* HUD = PC->GetHUD();
			if (AInGameHUDBase* InGameHUD = Cast<AInGameHUDBase>(HUD))
			{
				InGameHUD->InGameOverlay->SetMoney(Money);
			}
		}
	}
}
// Client Code
void UPickUpCompnent::IA_ToggleInventory()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AHUD* HUD = PC->GetHUD();
		if (AInGameHUDBase* InGameHUD = Cast<AInGameHUDBase>(HUD))
		{
			// 현재 상태를 확인하고 반대로 토글
			ESlateVisibility CurrentVis = InGameHUD->InGameOverlay->Inventory->GetVisibility();
			ESlateVisibility NewVis = (CurrentVis == ESlateVisibility::Visible) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
			InGameHUD->InGameOverlay->Inventory->SetVisibility(NewVis);

			// 마우스 커서 및 입력 모드 설정
			if (NewVis == ESlateVisibility::Visible)
			{
				// UI 모드로 설정, 마우스 커서 표시
				PC->SetInputMode(FInputModeUIOnly());
				PC->bShowMouseCursor = true;

				InGameHUD->InGameOverlay->Inventory->SetKeyboardFocus();

			}
			else
			{
				// 게임 모드로 설정, 마우스 커서 숨기기
				PC->SetInputMode(FInputModeGameOnly());
				PC->bShowMouseCursor = false;
			}
		}
	}
}

void UPickUpCompnent::ShowItemPickUpUI(ADS_Item* Item)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AHUD* HUD = PC->GetHUD();
		if (AInGameHUDBase* InGameHUD = Cast<AInGameHUDBase>(HUD))
		{
			InGameHUD->InGameOverlay->PickUp->SetVisibility(ESlateVisibility::Visible);
			if (Item)
			{
				InGameHUD->InGameOverlay->PickUp->ItemInfo->Item = Item->Item;
				InGameHUD->InGameOverlay->PickUp->ItemInfo->SetItemInfo();
				
			}
		}
	}
}
void UPickUpCompnent::HideItemPickUpUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AHUD* HUD = PC->GetHUD();
		if (AInGameHUDBase* InGameHUD = Cast<AInGameHUDBase>(HUD))
		{
			InGameHUD->InGameOverlay->PickUp->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
void UPickUpCompnent::OnCapsuleCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ADS_Item* Item = Cast<ADS_Item>(OtherActor);
	
		if (Item)
		{

			Client_ShowItemPickUpUI(Item);
			OverlappedItems.Add(Item);
		}
}
void UPickUpCompnent::OnCapsuleCompEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADS_Item* Item = Cast<ADS_Item>(OtherActor);
	if (Item)
	{
		Client_HiddenItemPickUpUI();
		OverlappedItems.Remove(Item);

	}
}

void UPickUpCompnent::Client_ShowItemPickUpUI_Implementation(ADS_Item* Item)
{
	ShowItemPickUpUI(Item);
}
void UPickUpCompnent::Client_HiddenItemPickUpUI_Implementation()
{
	HideItemPickUpUI();
}

void UPickUpCompnent::IA_PickUpItem()
{
	Server_PickUpItem();
}

void UPickUpCompnent::IA_QuitGame(){

	// 로비맵으로 이동
	GetWorld()->GetFirstPlayerController()->ClientTravel(TEXT("GDH_Lobby"), ETravelType::TRAVEL_Absolute);
}

void UPickUpCompnent::Server_PickUpItem_Implementation()
{

	for (auto Item : OverlappedItems)
	{
		
		Client_PickUpItem(Item->Item, Item);
		Net_Multicast_ItemSound();


	}
}

void UPickUpCompnent::Client_PickUpItem_Implementation(const FSlot& Item, ADS_Item* ItemToPickup)
{

	bool bIsShould = AddToItem(Item);
	// LOGA("픽업중 (클라) %s",*ItemToPickup->GetName())
	Server_IsShouldDestroyItem(bIsShould, ItemToPickup);
	if (bIsShould)
	{
		// 아이템 줍는 소리 추가
		UpdateInventoryUI();
	}
}

void UPickUpCompnent::Server_IsShouldDestroyItem_Implementation(bool bIsShould, ADS_Item* ItemToPickup){

	//LOGA("서버 픽업 확인 후 %s",*ItemToPickup->GetName())

	if (bIsShould && ItemToPickup)
	{

		ItemToPickup->Destroy();
	}
}

bool UPickUpCompnent::AddToItem(const FSlot& Item){


	switch (Item.ItemType)
	{
		case  EItemType::None:
			UE_LOG(LogTemp, Display, TEXT("Item type is None"));
		break;
		
		case  EItemType::Sword:
			return CreateNewSlotWithEquipment(Item, EItemType::Sword);
		
		case  EItemType::Shield:
			return CreateNewSlotWithEquipment(Item, EItemType::Shield);
			
		case EItemType::Eatables:

			// 아이템이 같은 종류가 있는지
		{
			bool bValue;
			if (AddQuantitySlot(Item,bValue)) return bValue;
		}
		// 아이템이 슬롯이 빈 곳을 찾기
		{
			bool bValue;
			if (CreateNewSlot(Item,bValue)) return bValue;
		}

		break;
		
	}
	return false;
		
}

bool UPickUpCompnent::AddQuantitySlot(const FSlot& Item, bool& bValue)
{
	for (auto& Eatable : AllItem.Eatables)
	{
		if (Eatable.ItemTableRowHandle.RowName == Item.ItemTableRowHandle.RowName)
		{

			FItem* DataRow = Item.ItemTableRowHandle.DataTable->FindRow<FItem>(Item.ItemTableRowHandle.RowName, TEXT(""));
			// 해당 아이템 슬롯 찾았고, 수량이 들어갈수 있을때
			if (DataRow->StackSize >= (Eatable.Quantity + Item.Quantity))
			{
				// 용량이 남지만, 넣었을때 넘는 경우, 가득찬 슬롯을 만들기는 나중에 ..
				Eatable.Quantity += Item.Quantity;

				bValue = true;
				return true;
			}
			// 해당 아이템 슬롯 찾았고, 수량이 가득 찼을때 일단 넘겨..
			else
			{
				continue;
			}
		}
	}return false;

}

bool UPickUpCompnent::CreateNewSlot(const FSlot& Item, bool& bValue)
{
	for (auto& Eatable : AllItem.Eatables)
	{
		if (Eatable.ItemType == EItemType::None)
		{
			// 그냥 ref 말고, 변수 밀어 넣기는 안되려나?
			// 이 방식은 아이템을 하나씩 받을때만 유효, 한번에 여러개 받을 시, ex) 100개면 넘게 찰 수 도 있음.
			Eatable.ItemTableRowHandle = Item.ItemTableRowHandle;
			Eatable.Quantity = Item.Quantity;
			Eatable.ItemType = EItemType::Eatables;
			bValue = true;
			return true;
					
		}
	}
	return false;
}

bool UPickUpCompnent::CreateNewSlotWithEquipment(const FSlot& Item, EItemType Type){

	TArray<FSlot>* TempArray = nullptr;
	
	if (Type == EItemType::Sword)	TempArray = &AllItem.Swords;
	if (Type == EItemType::Shield)	TempArray = &AllItem.Shields;

	check(TempArray);
	for (auto& Shield : *TempArray)
	{
		if (Shield.ItemType == EItemType::None)
		{
			Shield.ItemTableRowHandle = Item.ItemTableRowHandle;
			Shield.Quantity = Item.Quantity;
			Shield.ItemType = Type;
			return true;
		}
	}

	return false;
}

void UPickUpCompnent::UpdateInventoryUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AHUD* HUD = PC->GetHUD();
		if (AInGameHUDBase* InGameHUD = Cast<AInGameHUDBase>(HUD))
		{
			// 인벤토리 슬롯 UI 초기화
			InGameHUD->InGameOverlay->Inventory->UpdateItemsInInventoryUI(AllItem);
		}
	}
}
void UPickUpCompnent::OnJump()
{
	//	Server_Jump();
	if (Character)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (UWarriorHeroAnimInstance* WarriorHeroAnimInstance = Cast<UWarriorHeroAnimInstance>(AnimInstance))
		{
			//LOGA("애님인스턴스 호출")
			//WarriorHeroAnimInstance->AnimNotify_OnDie();
		}
	
		Character->Jump();
	}
}

void UPickUpCompnent::PlayBackgroundMusic(){

		if (BackgroundSound)
		{
			// 재생시
			PlayingSoundComponent = UGameplayStatics::CreateSound2D(GetWorld(), BackgroundSound);
			if (PlayingSoundComponent)
			{
				// 보스를 찾자.
				if (AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), BlueprintActorClass))
				{

					if (Cast<AWarriorEnemyCharacter>(Actor)->bIsOnce == true)
					{
						PlayingSoundComponent->Play();
						GetWorld()->GetTimerManager().SetTimer(SoundTimerHandle, this, &UPickUpCompnent::StopSound, 60.0f, false);
						
					}
				}
			}
		}
	
}

void UPickUpCompnent::StopSound()
{

	// 정지시
	if (PlayingSoundComponent && IsValid(PlayingSoundComponent))
	{
		PlayingSoundComponent->Stop();
	}
}

void UPickUpCompnent::Net_Multicast_DropSound_Implementation(){
	if (DropSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DropSound, GetOwner()->GetActorLocation());
	}
	
}

void UPickUpCompnent::Net_Multicast_ItemSound_Implementation(){
	if (ItemSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ItemSound, GetOwner()->GetActorLocation());
	}
}

void UPickUpCompnent::Server_Jump_Implementation(){

	if (Character)
	{
 		Character->Jump();
	}
}

void UPickUpCompnent::ForceRespawn(){
	Cast<AWarriorHeroCharacter>(GetOwner())->ReStartRespawn();

}