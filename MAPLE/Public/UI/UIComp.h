// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Base/InputComp.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "UIComp.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAPLE_API UUIComp : public UInputComp
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUIComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Enter;

public:
	void IA_EnterAction();

	UFUNCTION(Server, Reliable)
	void Server_SendMsg(const FText& Text, const FText& Username);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SendMsg(const FText& Text, const FText& Username);
};
