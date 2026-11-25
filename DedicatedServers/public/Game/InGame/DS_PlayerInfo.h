// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "DS_PlayerInfo.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FDS_PlayerInfo : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FDS_PlayerInfo() {}
	FDS_PlayerInfo(const FString& Name) : Username(Name) {}

	UPROPERTY(BlueprintReadWrite)
	FString Username{};
};

USTRUCT()
struct FDS_PlayerInfoArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDS_PlayerInfo> Players;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FDS_PlayerInfo, FDS_PlayerInfoArray>(Players, DeltaParams, *this);
	}

	void AddPlayer(const FDS_PlayerInfo& NewPlayerInfo);
	void RemovePlayer(const FString& Username);
};

template<>
struct TStructOpsTypeTraits<FDS_PlayerInfoArray> : public TStructOpsTypeTraitsBase2<FDS_PlayerInfoArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

