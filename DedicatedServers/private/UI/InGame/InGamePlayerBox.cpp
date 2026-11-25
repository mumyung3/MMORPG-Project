// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGame/InGamePlayerBox.h"

#include "Components/ScrollBox.h"
#include "Game/InGame/DSGameState.h"
#include "Game/InGame/DS_InGameState.h"
#include "UI/InGame/PlayerLabel.h"
void UInGamePlayerBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ADSGameState* DSGameState = GetWorld()->GetGameState<ADSGameState>();
	if (!IsValid(DSGameState)) return;

	if (IsValid(DSGameState->InGameState))
	{
		OnInGameStateInitialized(DSGameState->InGameState);
	}
	else
	{
		DSGameState->OnInGameStateInitialized.AddDynamic(this, &UInGamePlayerBox::OnInGameStateInitialized);
	}
}

void UInGamePlayerBox::UpdatePlayerInfo(ADS_InGameState* InGameState)
{
	ScrollBox_PlayerInfo->ClearChildren();
	for (const FDS_PlayerInfo& PlayerInfo : InGameState->GetPlayers())
	{
		CreateAndAddPlayerLabel(PlayerInfo);
	}
}

void UInGamePlayerBox::OnInGameStateInitialized(ADS_InGameState* InGameState)
{
	if (!IsValid(InGameState)) return;

	InGameState->OnPlayerInfoAdded.AddDynamic(this, &UInGamePlayerBox::CreateAndAddPlayerLabel);
	InGameState->OnPlayerInfoRemoved.AddDynamic(this, &UInGamePlayerBox::OnPlayerRemoved);
	UpdatePlayerInfo(InGameState);
}

void UInGamePlayerBox::CreateAndAddPlayerLabel(const FDS_PlayerInfo& PlayerInfo)
{
	if (FindPlayerLabel(PlayerInfo.Username)) return;
	
	UPlayerLabel* PlayerLabel = CreateWidget<UPlayerLabel>(this, PlayerLabelClass);
	if (!IsValid(PlayerLabel)) return;
	
	PlayerLabel->SetUsername(PlayerInfo.Username);
	ScrollBox_PlayerInfo->AddChild(PlayerLabel);
}

void UInGamePlayerBox::OnPlayerRemoved(const FDS_PlayerInfo& PlayerInfo)
{
	if (UPlayerLabel* PlayerLabel = FindPlayerLabel(PlayerInfo.Username))
	{
		ScrollBox_PlayerInfo->RemoveChild(PlayerLabel);
	}
}
UPlayerLabel* UInGamePlayerBox::FindPlayerLabel(const FString& Username)
{
	for (UWidget* Child : ScrollBox_PlayerInfo->GetAllChildren())
{
	UPlayerLabel* PlayerLabel = Cast<UPlayerLabel>(Child);
	if (IsValid(PlayerLabel) && PlayerLabel->GetUsername() == Username)
	{
		return PlayerLabel;
	}
}
	return nullptr;
}