// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/GameEnums.h"
#include "NPCDialogDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MAPLE_API UNPCDialogDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName NPCID;
	
	UPROPERTY(EditAnywhere)
	FText NPCName;

	UPROPERTY(EditAnywhere)
	ENPCRole NPCRole;
	
	UPROPERTY(EditAnywhere)
	TArray<FText> DialogueLines;
};
