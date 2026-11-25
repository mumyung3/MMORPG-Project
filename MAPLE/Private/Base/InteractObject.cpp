// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/InteractObject.h"
#include "Components/InteractableComponent.h"

// Sets default values
AInteractObject::AInteractObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Interactable = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	Interactable->OnEnterInteractDelegate.AddDynamic(this, &AInteractObject::OnEnterInteract);
	Interactable->OnInteractDelegate.AddDynamic(this, &AInteractObject::OnInteract);
	Interactable->OnExitInteractDelegate.AddDynamic(this, &AInteractObject::OnExitInteract);
}

void AInteractObject::OnEnterInteract(APawn* Interactor)
{
	unimplemented();
}

void AInteractObject::OnInteract()
{
	// unimplemented();
}

void AInteractObject::OnExitInteract()
{
	// unimplemented();
}

FText AInteractObject::GetInteractDesc()
{
	return FText::GetEmpty();
}

FText AInteractObject::GetInputKey()
{
	return InteractInputKey;
}

// Called when the game starts or when spawned
void AInteractObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
