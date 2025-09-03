// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/InputComp.h"
#include "InteractInputComp.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API UInteractInputComp : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	class UMain* CachedMainUI;

public:
	UInteractInputComp();

	/** Interact Function */
	void BeginInteraction();
	void EndInteraction();

	// Server RPC
	UFUNCTION(Server, Reliable)
	void ServerInteract(UInteractableComponent* Interactable);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* GetTargetObject();

	void OnUIOpened(bool bIsOpened);

	UFUNCTION()
	void HandleMainUIReady(UMain* MainUI);

protected:
	/** Called for Trace */
	void SphereTrace();

	virtual void BeginPlay() override;

private:
	bool bIsDetected;
	bool bBlockInteract = false; // UI가 열려있다면 true(INTERACTION BLOCK)

	UPROPERTY() // 캐릭터를 담고있는 포인터
	AWarriorHeroCharacter* CachedHeroCharacter = nullptr;

	// Detected target by trace
	TObjectPtr<AActor> target;
};
