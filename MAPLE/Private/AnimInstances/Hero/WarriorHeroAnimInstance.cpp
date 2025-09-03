// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/Combat/RespawnComponent.h"
#include "Controllers/WarriorHeroController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/Manger/UseItemComponent.h"
#include "Util/CustomLog.h"

void UWarriorHeroAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	if (OwningCharacter) {
		OwningHeroCharacter = Cast<AWarriorHeroCharacter>(OwningCharacter);
	}

}

void UWarriorHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds) {
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	
	if (bHasAcceleration) {
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else {
		IdleElpasedTime += DeltaSeconds;

		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter) {
		bIsJump = OwnerCharacter->GetCharacterMovement()->IsFalling();
	}

}

void UWarriorHeroAnimInstance::AnimNotify_OnDie(){


	// 클라면서 나 자신만
	if (GetOwningActor()->HasAuthority() || !Cast<APawn>(GetOwningActor())->IsLocallyControlled())return;

	if (APawn* Owner = TryGetPawnOwner())
	{
		
		if (AController* Controller = Owner->GetController())
		{
			UE_LOG(LogTemp, Warning, TEXT("Controller Class: %s"), *Controller->GetClass()->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Controller is nullptr"));
		}
		
		if (AWarriorHeroController* PC = Cast<AWarriorHeroController>(Owner->GetController()))
		{

			AWarriorHeroCharacter* WarriorHeroCharacter	= Cast<AWarriorHeroCharacter>(Owner);
			if (!WarriorHeroCharacter) return;
			WarriorHeroCharacter->PickUpComp->DropAllItem();
			// 사용
			PC->RespawnComponent->OnDeath();
		}
	}

}

