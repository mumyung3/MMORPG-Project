// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);
/**
 * 
 */
UCLASS()
class MAPLE_API UAbilityTask_ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAbilityTask_ExecuteTaskOnTick(const FObjectInitializer& ObjectInitializer);

	virtual void Activate() override;
	
	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_ExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	//~ Begin UGameplayTask Interface
	virtual void TickTask(float DeltaTime) override;
	//~ End UGameplayTask Interface

	virtual void OnDestroy(bool AbilityIsEnding) override;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
};
