// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/KeyBase.h"

AKeyBase::AKeyBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemID = "KEY";
	ItemName = "KEY";

	// KeyMesh
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetupAttachment(RootComponent);
}

void AKeyBase::OnEnterInteract(APawn* Interactor)
{
	Super::OnEnterInteract(Interactor);
	Destroy();
}

void AKeyBase::OnInteract()
{
}

void AKeyBase::OnExitInteract()
{
}

void AKeyBase::BeginPlay()
{
	Super::BeginPlay();
}
