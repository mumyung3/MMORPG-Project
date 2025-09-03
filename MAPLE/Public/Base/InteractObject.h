// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "InteractObject.generated.h"

UCLASS()
class MAPLE_API AInteractObject : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Interactions")
	class UInteractableComponent* Interactable;

public:
	// Sets default values for this actor's properties
	AInteractObject();

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
};
