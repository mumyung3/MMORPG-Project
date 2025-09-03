// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_PlayerInfo.h"
#include "GameFramework/Info.h"
#include "DS_InGameState.generated.h"

USTRUCT()
struct FDS_PlayerInfoDelta
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDS_PlayerInfo> AddedPlayers{};

	UPROPERTY()
	TArray<FDS_PlayerInfo> RemovedPlayers{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoChanged, const FDS_PlayerInfo&, PlayerInfo);

UCLASS()
class DEDICATEDSERVERS_API ADS_InGameState : public AInfo
{
	GENERATED_BODY()

public:
	ADS_InGameState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoAdded;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoRemoved;

	void AddPlayerInfo(const FDS_PlayerInfo& PlayerInfo);
	void RemovePlayerInfo(const FString& Username);
	TArray<FDS_PlayerInfo> GetPlayers();
protected:

	UFUNCTION()
	void OnRep_FDS_PlayerInfo();
	
private:

	UPROPERTY(ReplicatedUsing=OnRep_FDS_PlayerInfo)
	FDS_PlayerInfoArray PlayerInfoArray;
	
	UPROPERTY()
	FDS_PlayerInfoArray LastPlayerInfoArray;

	FDS_PlayerInfoDelta ComputePlayerInfoDelta(const TArray<FDS_PlayerInfo>& OldArray, const TArray<FDS_PlayerInfo>& NewArray);
};
