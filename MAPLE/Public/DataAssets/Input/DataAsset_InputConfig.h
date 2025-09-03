// Fill out your copyright notice in the Description page of Project Settings. c

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FWarriorInputActionConfig {		//WarriorInputActionConfig has Tag and InputAction!!
	GENERATED_BODY()

public: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;

	bool IsValid() const {
		return InputTag.IsValid() && InputAction;
	}
};
/**
 * 
 */
UCLASS()
class MAPLE_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag")) //InputAction <> InputTag
	TArray<FWarriorInputActionConfig> NativeInputActions;	//is array for Tag and InputAction

	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag)const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag")) //InputAction <> InputTag
	TArray<FWarriorInputActionConfig> AbilityInputActions;	//is array for Tag and InputAction
};
