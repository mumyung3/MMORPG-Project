// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragAndDropComponent.h"
#include "PickUpCompnent.h"
#include "UseItemComponent.generated.h"

class AWarriorHeroWeapon;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEDICATEDSERVERS_API UUseItemComponent : public UDragAndDropComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUseItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void UseItem(int32 Index, EItemType Type);

	UFUNCTION()
	void UseSword(int32 Index, EItemType Type);

	UFUNCTION()
	void UseShield(int32 Index, EItemType Type);

	UFUNCTION(Server , Reliable)
	void ServerRequestHeal(const FSlot& slot);
	
	void UseEatable(int32 Index, EItemType Type);

	UFUNCTION()
	void OnDrop(int32 Index, EItemType Type);

public:
	UPROPERTY()
	FSlot Eatable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Power{};

	AWarriorHeroWeapon* GetSwordActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_InitWeapon)
	AWarriorHeroWeapon* WarriorHeroWeapon;
	UFUNCTION()
	void OnRep_InitWeapon();

	UFUNCTION(Server, Reliable)
	void Server_OnWeaponEquipped(const FItem& Item);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnWeaponEquipped(const FItem& Item);

	UFUNCTION(Server, Reliable)
	void Server_OnShieldEquipped(const FItem& Item);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnShieldEquipped(const FItem& Item);

	UFUNCTION(Server, Reliable)
	void Server_DestroyWeapon();
	
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 월드에서 제거될 때 호출
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	

};
