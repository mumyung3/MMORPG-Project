// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

struct FInteractionInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnterInteractDelegate, APawn*, Interactor); // 매개변수 : 상호작용 주체 플레이어의 PC
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitInteractDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAPLE_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()
	// 해당 컴포넌트는 상호작용이 가능한 오브젝트에 붙여 사용함.
public:
	// Sets default values for this component's properties
	UInteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Interactions")
	FOnEnterInteractDelegate OnEnterInteractDelegate; // 상호작용 키를 눌렀을 때 콜백

	UPROPERTY(BlueprintAssignable, Category = "Interactions")
	FOnInteractDelegate OnInteractDelegate; // 상호작용 키를 누르고 있을 때 콜백

	UPROPERTY(BlueprintAssignable, Category = "Interactions")
	FOnExitInteractDelegate OnExitInteractDelegate; // 상호작용 키를 뗐을 때 콜백
};