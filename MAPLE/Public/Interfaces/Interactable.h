// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
// 해당 인터페이스는 상호작용이 가능한 오브젝트가 상속받게 하여 아래의 함수들을 구현하도록 함.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MAPLE_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void OnEnterInteract(APawn* Interactor) = 0; // When the Interaction Starts.
	virtual void OnInteract() = 0;		// When the Interaction is Going on.
	virtual void OnExitInteract() = 0;	// When the Interaction is Ended.

	virtual FText GetInteractDesc() = 0; // Return the Interaction Description.
	virtual FText GetInputKey() = 0; // Return the Interaction Input Key.
};