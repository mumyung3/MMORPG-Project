// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AbilityTask_ExecuteTaskOnTick.h"

UAbilityTask_ExecuteTaskOnTick::UAbilityTask_ExecuteTaskOnTick(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
	bSimulatedTask = true;

	UE_LOG(LogTemp, Warning, TEXT("Into UAbilityTask_ExecuteTaskOnTick : bTickingTask = %d"), bTickingTask);
}

void UAbilityTask_ExecuteTaskOnTick::Activate()
{
	
}

void UAbilityTask_ExecuteTaskOnTick::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);
}

UAbilityTask_ExecuteTaskOnTick* UAbilityTask_ExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	UE_LOG(LogTemp, Warning, TEXT("Into ExecuteTaskOnTick"));
	UAbilityTask_ExecuteTaskOnTick* Node = NewAbilityTask<UAbilityTask_ExecuteTaskOnTick>(OwningAbility);
	UE_LOG(LogTemp, Warning, TEXT("OwningAbility is : %s"), *OwningAbility->GetName());

	return Node;
}

void UAbilityTask_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Into TickTask"));
	Super::TickTask(DeltaTime);

	if (ShouldBroadcastAbilityTaskDelegates()) {
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else {
		EndTask();
	}
}

void UAbilityTask_ExecuteTaskOnTick::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}
