// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/LockPick.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Util/CustomLog.h"

void ULockPick::InitSetting()
{
	// Set the LockPick Success Zone
	SuccessLockPickRange = UKismetMathLibrary::RandomFloatInRange(0.1f, 0.32f);
	LockPickBar->SetPercent(SuccessLockPickRange);
}

UImage* ULockPick::getLockPickTiming()
{
	return LockPickTiming;
}

void ULockPick::PlayMovingLockPickTimingAnim()
{
	InitSetting();
	PlayAnimation(MovingLockPickTiming, 0, 0);
}

bool ULockPick::StopMovingLockPickTimingAnim()
{
	StopAnimation(MovingLockPickTiming);

	float LockPickTimingPosX = LockPickTiming->GetRenderTransform().Translation.X; // LockPickTiming Position X
	float LockPickBarWidth = LockPickBar->GetCachedGeometry().GetLocalSize().X;	   // LockPickBar Width
	float StopPer = abs(LockPickTimingPosX / (LockPickBarWidth / 2.0f));		   // Pencentage of Distance to Center
	float SuccPer = LockPickBar->GetPercent();									   // LockPicking Success Zone

	if (StopPer < SuccPer)
		return true;
	else
		return false;
}
