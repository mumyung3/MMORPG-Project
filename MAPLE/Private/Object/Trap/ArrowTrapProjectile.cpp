// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Trap/ArrowTrapProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "WarriorGameplayTags.h"
#include "Character/WarriorEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Util/CustomLog.h"

// Sets default values
AArrowTrapProjectile::AArrowTrapProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetGenerateOverlapEvents(true);
	RootComponent = CollisionBox;
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 7.f;
}

void AArrowTrapProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AArrowTrapProjectile::OnCollisionBoxBeginOverlap);
	CollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);

	// 0.5초 후 WorldStatic과의 콜리전을 Block으로 전환
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			if (IsValid(CollisionBox))
			{
				CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			}
		},
		0.5f,
		false
	);
}

void AArrowTrapProjectile::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (!HitPawn) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitPawn);
	if (!TargetASC || !ArrowProjectileEffectClass) return;

	// 여기서 직접 SpecHandle 생성
	FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
	Context.AddSourceObject(this); // 발사체를 출처로
	
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(ArrowProjectileEffectClass, 1.0f, Context);

	FGameplayEventData EventData;
	EventData.Instigator = this;
	EventData.Target = HitPawn;
	
	if (SpecHandle.IsValid())
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			WarriorGameplayTags::Shared_Event_HitReact,
			EventData
		);
	}
	
	if (auto HitChar = Cast<AWarriorEnemyCharacter>(HitPawn))
	{
		UGameplayStatics::PlaySoundAtLocation(
    		this,
    		EnemyHitSound,
    		GetActorLocation(),
    		1.0f,
    		FMath::FRandRange(0.9f, 1.1f),
    		0.0f,
    		AttenuationSettings
    	);
	}

	Destroy();
}

void AArrowTrapProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 충돌 시 Overlap 끄기
	CollisionBox->SetGenerateOverlapEvents(false); // 겹침 감지 자체를 끔
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore); // 모든 반응 제거
}