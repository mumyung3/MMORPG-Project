// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/LockPickInputComp.h"
#include "Input/InteractInputComp.h"
#include "UI/LockPick.h"
#include "Util/CustomLog.h"
#include "Character/WarriorHeroCharacter.h"
#include "Object/Door.h"

ULockPickInputComp::ULockPickInputComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIsLockPicking = false;

	TargetDoor = nullptr;
}

void ULockPickInputComp::ServerLockPick_Implementation(class ADoor* ServerTargetDoor)
{
	if (ServerTargetDoor)
		ServerTargetDoor->SetDoorUnlocked();
}

void ULockPickInputComp::BeginPlay()
{
	Super::BeginPlay();

	CachedHeroCharacter = Cast<AWarriorHeroCharacter>(GetOwner());
}

void ULockPickInputComp::LockPick()
{
	TargetDoor = Cast<ADoor>(Cast<UInteractInputComp>(CachedHeroCharacter->GetInteractInputComp())->GetTargetObject());

	if (TargetDoor) // Player is Aiming the Door and Pressed LockPick Button
	{
		if (TargetDoor->IsDoorLocked()) // Door is Locked
		{
			if (!bIsLockPicking) // Show the LockPick Widget
			{
				if (LockPickUIFactory)
				{
					LockPickUI = Cast<ULockPick>(CreateWidget(GetWorld(), LockPickUIFactory)); // CreateWidget is Heavy Function, so This Needs to be Improved.
					if (LockPickUI)
					{
						LockPickUI->AddToViewport();

						LockPickUI->PlayMovingLockPickTimingAnim();
					}
				}
			}
			else // Show the LockPicking Result
			{
				if (LockPickUI->StopMovingLockPickTimingAnim()) // LockPicking Success
				{
					if (GetOwnerRole() == ROLE_AutonomousProxy)
						ServerLockPick(TargetDoor);
				}

				/*
				The Delay Function is Necessary to Show the LockPick Result.
				*/

				LockPickUI->RemoveFromParent();
			}
			bIsLockPicking = !bIsLockPicking;
		}
	}
}
