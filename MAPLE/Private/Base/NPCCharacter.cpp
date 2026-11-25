// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/NPCCharacter.h"
#include "Components/InteractableComponent.h"

// Sets default values
ANPCCharacter::ANPCCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Interactable = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	Interactable->OnEnterInteractDelegate.AddDynamic(this, &ANPCCharacter::OnEnterInteract);
	Interactable->OnInteractDelegate.AddDynamic(this, &ANPCCharacter::OnInteract);
	Interactable->OnExitInteractDelegate.AddDynamic(this, &ANPCCharacter::OnExitInteract);
}

void ANPCCharacter::OnEnterInteract(APawn* Interactor)
{
	unimplemented();
}

void ANPCCharacter::OnInteract()
{
	// unimplemented();
}

void ANPCCharacter::OnExitInteract()
{
	// unimplemented();
}

FText ANPCCharacter::GetInteractDesc()
{
	return FText::GetEmpty();
}

FText ANPCCharacter::GetInputKey()
{
	return InteractInputKey;
}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
