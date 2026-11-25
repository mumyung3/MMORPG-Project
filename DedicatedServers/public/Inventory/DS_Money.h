// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DS_Money.generated.h"

class UInterpToMovementComponent;
UCLASS()
class DEDICATEDSERVERS_API ADS_Money : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADS_Money();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DestroyMyself();

	// 새로운 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;
	
	// 에디터 처리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FDataTableRowHandle MoneyTableRowHandle;

public:
	// 에디터 처리
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Info")
	class UDataTable* MoneyDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	UInterpToMovementComponent* InterpToMovementComponent;

protected:
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	FTimerHandle DestroyTimerHandle;
};
