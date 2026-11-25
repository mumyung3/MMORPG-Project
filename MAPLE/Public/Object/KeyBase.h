// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/PickupItem.h"
#include "KeyBase.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API AKeyBase : public APickupItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* KeyMesh;

public:
	AKeyBase();

	virtual void OnEnterInteract(APawn* Interactor) override;
	virtual void OnInteract() override;
	virtual void OnExitInteract() override;

protected:
	virtual void BeginPlay() override;
};
