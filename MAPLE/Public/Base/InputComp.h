// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "BaseComp.h"
#include "InputComp.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAPLE_API UInputComp : public UBaseComp
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInputComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void
	TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION()
	virtual void SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent);

};
