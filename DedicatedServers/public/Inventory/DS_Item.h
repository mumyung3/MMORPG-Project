// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "GameFramework/Actor.h"
#include "DS_Item.generated.h"

class UInterpToMovementComponent;
UCLASS()
class DEDICATEDSERVERS_API ADS_Item : public AActor
{
	GENERATED_BODY()

public:
	ADS_Item();

protected:
	void DestroyMyself();
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;

public:
	// 에디터 처리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	class UDataTable* ItemDataTable;

	// 구조체
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info",ReplicatedUsing = OnRep_InitMesh)
	FSlot Item{};

	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	FTimerHandle DestroyTimerHandle;

public:
	UFUNCTION()
	void OnRep_InitMesh();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	bool bIsNotDestroy;
};
