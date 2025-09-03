// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/InGame/DS_InGameState.h"

#include "Net/UnrealNetwork.h"

ADS_InGameState::ADS_InGameState()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}
void ADS_InGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADS_InGameState, PlayerInfoArray);
}
// 빠른 직렬화기에 원소 넣기
void ADS_InGameState::AddPlayerInfo(const FDS_PlayerInfo& PlayerInfo)
{
	PlayerInfoArray.AddPlayer(PlayerInfo);

}
// 빠른 직렬화기에 원소 제거
void ADS_InGameState::RemovePlayerInfo(const FString& Username)
{
	PlayerInfoArray.RemovePlayer(Username);
}
TArray<FDS_PlayerInfo> ADS_InGameState::GetPlayers(){
	return PlayerInfoArray.Players;
}
void ADS_InGameState::OnRep_FDS_PlayerInfo()
{

	FDS_PlayerInfoDelta Delta = ComputePlayerInfoDelta(LastPlayerInfoArray.Players, PlayerInfoArray.Players);
	for (const auto& PlayerInfo : Delta.AddedPlayers)
	{
		OnPlayerInfoAdded.Broadcast(PlayerInfo);
	}
	for (const auto& PlayerInfo : Delta.RemovedPlayers)
	{
		OnPlayerInfoRemoved.Broadcast(PlayerInfo);
	}

	LastPlayerInfoArray = PlayerInfoArray;
}
FDS_PlayerInfoDelta ADS_InGameState::ComputePlayerInfoDelta(const TArray<FDS_PlayerInfo>& OldArray, const TArray<FDS_PlayerInfo>& NewArray){

	FDS_PlayerInfoDelta Delta;

	TMap<FString, const FDS_PlayerInfo*> OldMap;
	TMap<FString, const FDS_PlayerInfo*> NewMap;

	for (const auto& PlayerInfo : OldArray)
	{
		OldMap.Add(PlayerInfo.Username, &PlayerInfo);
	}
	for (const auto& PlayerInfo : NewArray)
	{
		NewMap.Add(PlayerInfo.Username, &PlayerInfo);
	}
	for (const auto& OldPlayerInfo : OldArray)
	{
		if (!NewMap.Contains(OldPlayerInfo.Username))
		{
			Delta.RemovedPlayers.Add(OldPlayerInfo);
		}
	}
	for (const auto& NewPlayerInfo : NewArray)
	{
		if (!OldMap.Contains(NewPlayerInfo.Username))
		{
			Delta.AddedPlayers.Add(NewPlayerInfo);
		}
	}

	return Delta;
}


