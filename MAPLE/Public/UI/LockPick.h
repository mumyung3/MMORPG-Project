// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockPick.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API ULockPick : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* LockPickBar;

	UPROPERTY(meta = (BindWidget))
	class UImage* LockPickTiming;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockPick UI Anim", meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* MovingLockPickTiming;

public:
	void InitSetting();

	UImage* getLockPickTiming();

	// LockPick Timing Bar Anim.
	void PlayMovingLockPickTimingAnim();
	bool StopMovingLockPickTimingAnim(); // LockPickTiming is in the Success Zone : T | else : F

private:
	float SuccessLockPickRange;
};
