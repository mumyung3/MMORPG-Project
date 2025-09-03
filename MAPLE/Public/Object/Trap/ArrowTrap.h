// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseTrap.h"
#include "ArrowTrap.generated.h"

/**
 * 
 */
UCLASS()
class MAPLE_API AArrowTrap : public ABaseTrap
{
	GENERATED_BODY()

public:
	AArrowTrap();

protected:
	virtual void TriggerTrap() override;

	// 화살을 생성할 위치
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ArrowSpawnPoint;

	// 어떤 화살을 생성할지
	UPROPERTY(EditAnywhere, Category = "Trap")
	TSubclassOf<class AArrowTrapProjectile> ArrowClass;

	// 화살 속도
	UPROPERTY(EditAnywhere, Category = "Trap")
	float ArrowSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundAttenuation* AttenuationSettings;
};
