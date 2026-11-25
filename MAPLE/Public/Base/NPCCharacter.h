// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Interactable.h"
#include "NPCCharacter.generated.h"

UCLASS()
class MAPLE_API ANPCCharacter : public ACharacter, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Interactions")
	class UInteractableComponent* Interactable;

public:
	// Sets default values for this character's properties
	ANPCCharacter();

	UFUNCTION(BlueprintCallable)
	virtual void OnEnterInteract(APawn* Interactor) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnExitInteract() override;

	UFUNCTION(BlueprintCallable)
	virtual FText GetInteractDesc() override;

	UFUNCTION(BlueprintCallable)
	virtual FText GetInputKey() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FText InteractDesc;
	FText InteractInputKey = FText::FromString(TEXT("F"));
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
