// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowTrapProjectile.generated.h"

UCLASS()
class MAPLE_API AArrowTrapProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;
	
public:
	AArrowTrapProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> ArrowProjectileEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "SFX")
    USoundBase* EnemyHitSound;
    
    UPROPERTY(EditDefaultsOnly, Category = "SFX")
    USoundAttenuation* AttenuationSettings;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
