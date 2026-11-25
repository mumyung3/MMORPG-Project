// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "Character/WarriorBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Net/UnrealNetwork.h"

void UWarriorCharacterAnimInstance::NativeInitializeAnimation() {
	OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());

	if (OwningCharacter) {
		OwngingMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds) {
	if (!OwningCharacter || !OwngingMovementComponent) {
		return;
	}
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
	bHasAcceleration = OwngingMovementComponent->GetCurrentAcceleration().SizeSquared2D()>0.f;
	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
	// NativeUpdateAnimation에서 직접 계산
	bHasAcceleration = OwningCharacter->GetCharacterMovement()->GetCurrentAcceleration().SizeSquared() > 0.0f;
}

void UWarriorCharacterAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(UWarriorCharacterAnimInstance, GroundSpeed);
	DOREPLIFETIME(UWarriorCharacterAnimInstance, bHasAcceleration);
	DOREPLIFETIME(UWarriorCharacterAnimInstance, LocomotionDirection);
}
