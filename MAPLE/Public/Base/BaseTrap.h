// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTrap.generated.h"

UCLASS()
class MAPLE_API ABaseTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseTrap();

	virtual void TriggerTrap();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool bReactive = true; // 함정이 발동 후 다시 활성화되는지 여부

	UPROPERTY(EditAnywhere)
	float CooldownTime = 3.f; // 함정이 다시 발동할 때까지의 쿨타임

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerArea; // 플레이어 감지 영역

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* TrapMesh; // 함정의 메쉬

	UFUNCTION()
	virtual void OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
