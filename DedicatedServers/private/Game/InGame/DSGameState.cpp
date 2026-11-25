// Fill out your copyright notice in the Description page of Project Settings.
#include "Game/InGame/DSGameState.h"
#include "Net/UnrealNetwork.h"
#include "Game/InGame/DS_InGameState.h"
ADSGameState::ADSGameState()
{
	bReplicates = true;
}

void ADSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADSGameState, InGameState);
}

void ADSGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CreateInGameState();
		OnInGameStateInitialized.Broadcast(InGameState);
	}
}

void ADSGameState::CreateInGameState()
{
	if (UWorld* World = GetWorld(); IsValid(World))
	{
		FActorSpawnParameters SpawnParams;
		InGameState = World->SpawnActor<ADS_InGameState>(
			ADS_InGameState::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		if (IsValid(InGameState))
		{
			InGameState->SetOwner(this);
		}
	}
}

void ADSGameState::OnRep_InGameState()
{
	OnInGameStateInitialized.Broadcast(InGameState);
}