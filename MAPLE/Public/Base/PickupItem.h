// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/InteractObject.h"
#include "PickupItem.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API APickupItem : public AInteractObject
{
	GENERATED_BODY()

public:
	APickupItem();

	virtual void OnEnterInteract(APawn* Interactor) override;
	virtual void OnInteract() override;
	virtual void OnExitInteract() override;

	virtual FText GetInteractDesc() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = "true"))
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = "true"))
	FString ItemName;

public:
	virtual void PickedUp();
};
