// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/Gate.h"

#include "Kismet/KismetMathLibrary.h"
#include "Util/CustomLog.h"

AGate::AGate()
{
	RightGateDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightGateDoorMesh"));
	RightGateDoorMesh->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void AGate::BeginPlay()
{
	Super::BeginPlay();

	// Rebind the UpdateTimline Function
	DoorOpenTimelineUpdateDeligate.Clear();
	DoorOpenTimelineUpdateDeligate.BindUFunction(this, FName("UpdateDoorTimeline"));
}

void AGate::UpdateDoorTimeline(float Output)
{
	Super::UpdateDoorTimeline(Output);

	FRotator RightGateDoorLerp = UKismetMathLibrary::RLerp(FRotator(0, 0, 0), FRotator(0, 90, 0), Output, true);
	RightGateDoorMesh->SetRelativeRotation(RightGateDoorLerp);
}