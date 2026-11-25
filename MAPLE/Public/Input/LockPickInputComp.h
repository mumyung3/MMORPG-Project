// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/InputComp.h"
#include "LockPickInputComp.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API ULockPickInputComp : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "LockPick UI")
	class TSubclassOf<UUserWidget> LockPickUIFactory;

	UPROPERTY(EditAnywhere, Category = "LockPick UI")
	class ULockPick* LockPickUI;

public:
	ULockPickInputComp();

	// Server RPC
	UFUNCTION(Server, Reliable)
	void ServerLockPick(class ADoor* ServerTargetDoor);

	void LockPick();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY() // 캐릭터를 담고있는 포인터
	AWarriorHeroCharacter* CachedHeroCharacter = nullptr;
	
	bool		 bIsLockPicking; // Player is LockPicking
	class ADoor* TargetDoor;	 // Door that Player is Aiming
};
