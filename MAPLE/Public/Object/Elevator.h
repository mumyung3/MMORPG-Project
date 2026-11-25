// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UCLASS()
class MAPLE_API AElevator : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Platform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PressurePlate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* PlayerDetector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	float Speed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	float MoveDistance = 500.0f;

	float MovedDistance = 0.0f;

	bool bIsTriggered = false;
	bool bIsUpward = false;	 // To Decide Move Up or Down
	bool bIsReached = false; // If the Elevator Reached to the End : T

public:
	// Sets default values for this actor's properties
	AElevator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MoveUp(float DeltaTime);

	UFUNCTION()
	void MoveDown(float DeltaTime);

	UFUNCTION()
	void StopCheck();
};
