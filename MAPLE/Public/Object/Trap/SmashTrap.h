// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseTrap.h"
#include "SmashTrap.generated.h"

/**
 * 
 */
UCLASS()
class MAPLE_API ASmashTrap : public ABaseTrap
{
	GENERATED_BODY()
	
public:
	ASmashTrap();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere) // 내려찍기 충돌 감지용 메쉬(원기둥)
	class UStaticMeshComponent* SmashCollisionMesh;

	UPROPERTY()
	class UTimelineComponent* SmashTimeline;

	UPROPERTY()
	TObjectPtr<class UCurveFloat> SmashTimelineCurve;

	UPROPERTY(EditAnywhere)
	UCurveFloat* SmashCurve;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> SmashEffectClass;

	UPROPERTY(EditAnywhere)
	float SmashSpeed = 1000.f;

	UPROPERTY(EditAnywhere)
	float SmashDistance = 490.f;

	FVector InitialLocation;
	FVector TargetLocation; // 내려찍는 위치

	FVector CollisionInitialLocation;
	FVector CollisionTargetLocation;

	bool bTriggered = false;
	bool bIsSmashed = false;
	bool bSoundPlayed = false;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* SmashSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundAttenuation* AttenuationSettings;

	virtual void TriggerTrap() override;

	UFUNCTION()
	void HandleTimelineProgress(float Value);

	UFUNCTION()
	void OnTimelineFinished();

	UFUNCTION()
	virtual void OnSmashCollisionMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
