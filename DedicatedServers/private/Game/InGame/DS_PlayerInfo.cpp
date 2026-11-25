// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/InGame/DS_PlayerInfo.h"


void FDS_PlayerInfoArray::AddPlayer(const FDS_PlayerInfo& NewPlayerInfo)
{
	int32 Index = Players.Add(NewPlayerInfo);
	MarkItemDirty(Players[Index]);
	Players[Index].PostReplicatedAdd(*this);
}

void FDS_PlayerInfoArray::RemovePlayer(const FString& Username)
{
	for (int32 PlayerIndex = 0; PlayerIndex < Players.Num(); ++PlayerIndex)
	{
		FDS_PlayerInfo& PlayerInfo = Players[PlayerIndex];
		if (PlayerInfo.Username == Username)
		{
			PlayerInfo.PreReplicatedRemove(*this);
			Players.RemoveAtSwap(PlayerIndex);
			MarkArrayDirty();
			break;
		}
	}
}
