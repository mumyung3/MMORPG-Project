// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SpawnManager.generated.h"

/**
 * 
 */
UCLASS(blueprintable)
class DEDICATEDSERVERS_API USpawnManager : public UObject
{
	GENERATED_BODY()

private:
	FTimerHandle TimerHandle;
public:
	void SpawnActor();

	void StartTimer();
	
	void SpawnActorsAroundAllPlayers(UObject* WorldContextObject, const TArray<TSubclassOf<AActor>>& SpawnedActors);
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> SpawnActors;
};
