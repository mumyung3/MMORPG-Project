// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameFramework/GameMode.h"
#include "DS_GameMode.generated.h"

class ANotificationManager;
class USpawnManager;
class UDS_GameInstanceSubsystem;
/**
 *
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	// Process Parameters needs to remain in scope for the lifetime of the app
	FProcessParameters m_params;
	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;
	
	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	void TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage);
	virtual void Logout(AController* Exiting) override;
	void RemovePlayerSession(AController* Exiting);
	void AddPlayerInfoToLobbyState(AController* Player) const;
	void RemovePlayerInfoFromLobbyState(AController* Player) const;
	virtual FString	InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;

	// SPAWN MANAGER
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USpawnManager> SpawnManagerClass;
	
	UPROPERTY()
	USpawnManager* SpawnManager;

	UPROPERTY(EditDefaultsOnly, Category = "info")
	TSubclassOf<ANotificationManager> NotificationManagerClass;

	UPROPERTY()
	ANotificationManager* NotificationManager;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
