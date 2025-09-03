// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "PickUpCompnent.generated.h"



class ADS_Item;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEDICATEDSERVERS_API UPickUpCompnent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPickUpCompnent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Boss")
	TSubclassOf<AActor> BlueprintActorClass;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void IA_ToggleInventory();
	void SetupPlayerInputComponent(UEnhancedInputComponent* Cast);

	UPROPERTY()
	class AWarriorHeroCharacter* Character;

	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	UInputAction* IA_KeyI;

	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	UInputAction* IA_KeyE;

	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	UInputAction* IA_KeySpace;
	
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	UInputAction* IA_KeyESC;
	
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	UInputAction* IA_KeyRespawn;
	
	UPROPERTY()
	UAudioComponent* PlayingSoundComponent;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_Money)
	int32 Money{};

	UFUNCTION()
	void OnRep_Money();

	void ShowItemPickUpUI(ADS_Item* Item);

	void HideItemPickUpUI();

	UFUNCTION()
	void OnCapsuleCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleCompEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Client, Reliable)
	void Client_ShowItemPickUpUI(ADS_Item* Item);

	UFUNCTION(Client, Reliable)
	void Client_HiddenItemPickUpUI();


	UPROPERTY(EditAnywhere, Category = "info")
	FALLItem AllItem{};

	bool CreateNewSlot(const FSlot& Item, bool& bValue);
	bool AddQuantitySlot(const FSlot& Item, bool& bValue);
	bool CreateNewSlotWithEquipment(const FSlot& Slot, EItemType Type);
	
	bool AddToItem(const FSlot& Item);
	
	UFUNCTION()
	void IA_PickUpItem();

	UFUNCTION()
	void IA_QuitGame();

	
	UFUNCTION(Server, Reliable)
	void Server_PickUpItem();

	UPROPERTY()
	TArray<ADS_Item*> OverlappedItems;
	
	UFUNCTION(Client, Reliable)
	void Client_PickUpItem(const FSlot& Item, ADS_Item* ItemToPickup);

	UFUNCTION(Server, Reliable)
	void Server_IsShouldDestroyItem(bool bIsShould, ADS_Item* ItemToPickup);
	
	void UpdateInventoryUI();

	void OnJump();

	UFUNCTION(Server, Reliable)
	void Server_Jump();

	// 헤더 파일에 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* DropSound;

	// 헤더 파일에 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* ItemSound;

	
	UFUNCTION(NetMulticast, Reliable)
	void Net_Multicast_DropSound();
	
	UFUNCTION(NetMulticast, Reliable)
	void Net_Multicast_ItemSound();

	UFUNCTION()
	void PlayBackgroundMusic();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* BackgroundSound;

	FTimerHandle SoundTimerHandle;
	
	void StopSound();

	void ForceRespawn();
};
