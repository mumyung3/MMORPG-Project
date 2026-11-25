// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NotificationManager.generated.h"

UCLASS()
class DEDICATEDSERVERS_API ANotificationManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANotificationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NETMulticast, Reliable)
	void MulticastPlayerJoined(const FString& PlayerName);
};
