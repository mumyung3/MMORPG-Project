// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Trap/SmashTrap.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "WarriorGameplayTags.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Util/CustomLog.h"

ASmashTrap::ASmashTrap()
{
	SmashCollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SmashCollisionMesh"));
	SmashCollisionMesh->SetupAttachment(RootComponent);
	
	SmashTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SmashTimeline"));
}

void ASmashTrap::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = TrapMesh->GetComponentLocation();
	TargetLocation = InitialLocation - FVector(0, 0, SmashDistance); // 아래로 찍기

	CollisionInitialLocation = SmashCollisionMesh->GetComponentLocation();
	CollisionTargetLocation = CollisionInitialLocation - FVector(0,0,SmashDistance);

	if (SmashCurve)
	{
		FOnTimelineFloat Progress;
		Progress.BindUFunction(this, FName("HandleTimelineProgress"));
		SmashTimeline->AddInterpFloat(SmashCurve, Progress);

		FOnTimelineEvent Finished;
		Finished.BindUFunction(this, FName("OnTimelineFinished"));
		SmashTimeline->SetTimelineFinishedFunc(Finished);

		SmashTimeline->SetLooping(false);
	}
	//중복 바인딩 문제 발견 임시 처리
	if (!SmashCollisionMesh->OnComponentBeginOverlap.IsBound())
		SmashCollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ASmashTrap::OnSmashCollisionMeshBeginOverlap);
}

void ASmashTrap::TriggerTrap()
{
	if (!bTriggered)
	{
		SmashTimeline->PlayFromStart();
		bTriggered = true;
	}
}

void ASmashTrap::HandleTimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, Value);
	TrapMesh->SetWorldLocation(NewLocation);

	FVector CollisionNewLocation = FMath::Lerp(CollisionInitialLocation, CollisionTargetLocation, Value);
	SmashCollisionMesh->SetWorldLocation(CollisionNewLocation);

	if (Value >= 1.0f)
	{
		bIsSmashed = true;
		if (!bSoundPlayed)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				SmashSound,
				GetActorLocation(),
				1.0f,
				FMath::FRandRange(0.9f, 1.1f),
				0.0f,
				AttenuationSettings
			);
			bSoundPlayed = true;
		}
	}
}

void ASmashTrap::OnTimelineFinished()
{
	bTriggered = false;
	bIsSmashed = false;
	bSoundPlayed = false;
}

void ASmashTrap::OnSmashCollisionMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsSmashed) return;
	
	APawn* HitPawn = Cast<APawn>(OtherActor);
	if (!HitPawn) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitPawn);
	if (!TargetASC || !SmashEffectClass) return;
	
	// 여기서 직접 SpecHandle 생성
	FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
	Context.AddSourceObject(this);
	
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(SmashEffectClass, 1.0f, Context);
	
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
}