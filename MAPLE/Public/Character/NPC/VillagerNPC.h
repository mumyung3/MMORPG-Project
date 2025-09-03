// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/NPCCharacter.h"
#include "VillagerNPC.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API AVillagerNPC : public ANPCCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
    class UNPCDialogDataAsset* DialogData;

public:
	AVillagerNPC();

	virtual void OnEnterInteract(APawn* Interactor) override;
	virtual void OnInteract() override;
	virtual void OnExitInteract() override;

	virtual FText GetInteractDesc() override;

protected:
	virtual void BeginPlay() override;
};
