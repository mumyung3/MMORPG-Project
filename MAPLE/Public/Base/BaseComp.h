// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseComp.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAPLE_API UBaseComp : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 모든 클라에서 플레이어들이 전부 담김.
	UPROPERTY()
	class AWarriorHeroCharacter* Player;

	// 각 클라의 컴퓨터에서 주인공만 담김.
	UPROPERTY()
	class AWarriorHeroController* PC;

protected:
	virtual void InitializeComponent() override;
};
