// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Trap/ArrowTrap.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Object/Trap/ArrowTrapProjectile.h"
#include "Util/CustomLog.h"

AArrowTrap::AArrowTrap()
{
	ArrowSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSpawnPoint"));
	ArrowSpawnPoint->SetupAttachment(RootComponent);
}

void AArrowTrap::TriggerTrap()
{
	if (!ArrowClass) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	
	FVector SpawnLocation = ArrowSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ArrowSpawnPoint->GetComponentRotation();

	AArrowTrapProjectile* Arrow = GetWorld()->SpawnActor<AArrowTrapProjectile>(ArrowClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Arrow)
	{
		FVector LaunchDirection = ArrowSpawnPoint->GetForwardVector();
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ShootSound,
			GetActorLocation(),
			1.0f,
			FMath::FRandRange(0.9f, 1.1f),
			0.0f,
			AttenuationSettings
		);
	}
}