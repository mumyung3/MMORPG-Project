// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/InteractInputComp.h"

#include "DrawDebughelpers.h"
#include "Base/InteractObject.h"
#include "Camera/CameraComponent.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/InteractableComponent.h"
#include "Controllers/WarriorHeroController.h"
#include "Util/CustomLog.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/IngameHUD.h"
#include "UI/Main.h"

UInteractInputComp::UInteractInputComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractInputComp::BeginInteraction()
{
	if (target)
	{
		UInteractableComponent* Interactable = target->GetComponentByClass<UInteractableComponent>();

		if (!Interactable)
		{
			return;
		}

		if (GetOwnerRole() == ROLE_AutonomousProxy)
			ServerInteract(Interactable);
	}
}

void UInteractInputComp::EndInteraction()
{
}

void UInteractInputComp::ServerInteract_Implementation(UInteractableComponent* Interactable)
{
	APawn* InteractorPawn = Cast<APawn>(GetOwner());
	
	if (!InteractorPawn || Interactable == nullptr)
		return;

	if (target)
	{
		Interactable->OnEnterInteractDelegate.Broadcast(InteractorPawn);
	}
}

void UInteractInputComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bBlockInteract) return;
	
	SphereTrace();
}

AActor* UInteractInputComp::GetTargetObject()
{
	if (target)
		return target;
	else
		return nullptr;
}

void UInteractInputComp::OnUIOpened(bool bIsOpened)
{
	bBlockInteract = bIsOpened;

	if (bIsOpened)
	{
		// 상호작용 UI OFF
		if (CachedMainUI)
			CachedMainUI->ShowInteractUI(false);
	}
}

void UInteractInputComp::HandleMainUIReady(UMain* MainUI)
{
	CachedMainUI = MainUI;
	if (CachedMainUI)
		LOGC("YES MAIN UI")
	else
		LOGC("NO MAIN UI")
}

void UInteractInputComp::SphereTrace()
{
	FVector			start = CachedHeroCharacter->GetFollowCamera()->GetComponentLocation();
	FVector			end = start + CachedHeroCharacter->GetFollowCamera()->GetForwardVector() * 500.0f;
	float			radius = 20.0f;
	ETraceTypeQuery traceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<AActor*> ActorsToIgnore;
	FHitResult		hitInfo;

	ActorsToIgnore.Add(CachedHeroCharacter); // 자신 무시

	bIsDetected = UKismetSystemLibrary::SphereTraceSingle(
		this,
		start,
		end,
		radius,						  // 구체 반지름
		traceType,					  // 감지할 충돌 채널
		false,						  // 복잡한 메시 감지
		ActorsToIgnore,				  // 무시할 액터
		EDrawDebugTrace::None,		// 디버그 라인
		hitInfo,					  // 충돌 결과 저장
		true						  // 자신 무시 여부
	);

	if (bIsDetected) // Character is looking an object.
	{
		target = hitInfo.GetActor();

		if (target->Implements<UInteractable>())
		{
			IInteractable* InteractableInterface = Cast<IInteractable>(target);
			if (InteractableInterface)
			{
				if (CachedMainUI)
				{
					CachedMainUI->SetInteractText(InteractableInterface->GetInteractDesc());
					CachedMainUI->SetInteractInputKey(InteractableInterface->GetInputKey());
					CachedMainUI->ShowInteractUI(true);
				}
			}
		}
	}
	else // Character is NOT looking any objects.
	{
		target = nullptr;

		if (CachedMainUI)
			CachedMainUI->ShowInteractUI(false);
	}
}

void UInteractInputComp::BeginPlay()
{
	Super::BeginPlay();

	APawn* InteractorPawn = Cast<APawn>(GetOwner());

	if (InteractorPawn)
	{
		AWarriorHeroController* PC = Cast<AWarriorHeroController>(InteractorPawn->GetController());
		if (PC)
		{
			if (AIngameHUD* HUD = Cast<AIngameHUD>(PC->GetIngameHUD()))
			{
				if (HUD->GetMainUI())
				{
					HandleMainUIReady(HUD->GetMainUI());
				}
				else
				{
					HUD->OnMainUIReady.AddDynamic(this, &UInteractInputComp::HandleMainUIReady);
				}
			}
		}

		CachedHeroCharacter = Cast<AWarriorHeroCharacter>(GetOwner());
	}
}