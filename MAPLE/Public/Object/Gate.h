// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Door.h"
#include "Gate.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API AGate : public ADoor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightGateDoorMesh;

	virtual void UpdateDoorTimeline(float Output) override;

public:
	AGate();

protected:
	virtual void BeginPlay() override;
};
