// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/DS_GameMode.h"

#include "DedicatedServers.h"
#include "Game/InGame/DSGameState.h"
#include "Game/InGame/DS_GamePlayerController.h"
#include "Game/InGame/DS_InGameState.h"
#include "Game/InGame/NotificationManager.h"
#include "Game/Shared/Game/DS_GameInstanceSubsystem.h"
#include "Inventory/Manger/SpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Util/CustomLog.h"

void ADS_GameMode::BeginPlay()
{
	Super::BeginPlay();

	InitGameLift();

	if (SpawnManagerClass) // SpawnManagerClass가 설정되었는지 확인
	{
		// NewObject로 UObject 생성
		SpawnManager = NewObject<USpawnManager>(this, SpawnManagerClass);

		if (SpawnManager)
		{
			UE_LOG(DedicatedServers, Log, TEXT("SpawnManager successfully created and initialized."));

			// 추가 초기화 로직 실행 가능
			SpawnManager->StartTimer();
		}
		else
		{
			UE_LOG(DedicatedServers, Warning, TEXT("Failed to create SpawnManager."));
		}
	}
	else
	{
		UE_LOG(DedicatedServers, Warning, TEXT("SpawnManagerClass is not set."));
	}


	FVector Location = FVector(0, 0, 100);
	FRotator Rotation = FRotator::ZeroRotator;
	// 노티파이 객체 생성
	// 가장 기본적인 스폰
	NotificationManager = GetWorld()->SpawnActor<ANotificationManager>(ANotificationManager::StaticClass(), Location, Rotation);

	
}
void ADS_GameMode::InitGameLift()
{
	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		if (DSGameInstanceSubsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>(); IsValid(DSGameInstanceSubsystem))
		{
			FServerParameters ServerParameters;
			SetServerParameters(ServerParameters);
			DSGameInstanceSubsystem->InitGameLift(ServerParameters);
		}
	}
}
void ADS_GameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	// AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(DedicatedServers, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	// The Host/compute-name of the GameLift Anywhere instance.
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(DedicatedServers, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}

	// The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(DedicatedServers, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}

	// The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(DedicatedServers, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}

	// The PID of the running process
	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
	UE_LOG(DedicatedServers, Log, TEXT("PID: %s"), *OutServerParameters.m_processId);
}

void ADS_GameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	TryAcceptPlayerSession(PlayerSessionId, Username, ErrorMessage);
}

FString ADS_GameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	if (ADS_GamePlayerController* DSPlayerController = Cast<ADS_GamePlayerController>(NewPlayerController); IsValid(DSPlayerController))
	{
		DSPlayerController->PlayerSessionId = PlayerSessionId;

		DSPlayerController->Username = Username;

		// test
		static int32 UsernameCounter = 1;
		if (GIsEditor)
		{
			DSPlayerController->Username = FString::Printf(TEXT("test%d"), UsernameCounter++);
		}

	}

	AddPlayerInfoToLobbyState(NewPlayerController);

	return InitializedString;
}
void ADS_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// PostLogin은 모든 초기화 완료 후 호출됨
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this, NewPlayer]()
	{
		if (NotificationManager)
		{

			if (ADS_GamePlayerController* DSPlayerController = Cast<ADS_GamePlayerController>(NewPlayer); IsValid(DSPlayerController))
			{
				FString PlayerName = DSPlayerController->Username;
				NotificationManager->MulticastPlayerJoined(PlayerName);
			}
		}
	}, 0.5f, false);  // 여유롭게 0.5초 지연
}

// 플레이어 세션 예약 -> 허용
void ADS_GameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage)
{
	
	/*if (PlayerSessionId.IsEmpty() || Username.IsEmpty())
	{
		//OutErrorMessage = TEXT("PlayerSessionId and/or Username invalid.");
		return;
	}*/

	#if WITH_GAMELIFT
	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest DescribePlayerSessionsRequest;
	DescribePlayerSessionsRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));

	const auto& DescribePlayerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(DescribePlayerSessionsRequest);
	if (!DescribePlayerSessionsOutcome.IsSuccess())
	{
		OutErrorMessage = TEXT("DescribePlayerSessions failed.");
		return;
	}

	const auto& DescribePlayerSessionsResult = DescribePlayerSessionsOutcome.GetResult();
	int32 Count = 0;
	const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = DescribePlayerSessionsResult.GetPlayerSessions(Count);
	if (PlayerSessions == nullptr || Count == 0)
	{
		OutErrorMessage = TEXT("GetPlayerSessions failed.");
		return;
	}

	for (int32 i = 0; i < Count; i++)
	{
		const Aws::GameLift::Server::Model::PlayerSession& PlayerSession = PlayerSessions[i];
		if (!Username.Equals(PlayerSession.GetPlayerId())) continue;
		if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
		{
			OutErrorMessage = FString::Printf(TEXT("Session for %s not RESERVED; Fail PreLogin."), *Username);
			return;
		}

		const auto& AcceptPlayerSessionOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
		OutErrorMessage = AcceptPlayerSessionOutcome.IsSuccess() ? "" : FString::Printf(TEXT("Failed to accept player session for %s"), *Username);
	}
	#endif
}

// 플레이어 세션 허용 -> 종료 
void ADS_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerSession(Exiting);
	RemovePlayerInfoFromLobbyState(Exiting);
}

void ADS_GameMode::RemovePlayerSession(AController* Exiting)
{
	ADS_GamePlayerController* DSGamePlayerController = Cast<ADS_GamePlayerController>(Exiting);
	if (!IsValid(DSGamePlayerController)) return;

	#if WITH_GAMELIFT
	const FString& PlayerSessionId = DSGamePlayerController->PlayerSessionId;
	if (!PlayerSessionId.IsEmpty())
	{
		Aws::GameLift::Server::RemovePlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
	}
	#endif
}


void ADS_GameMode::AddPlayerInfoToLobbyState(AController* Player) const
{
	ADS_GamePlayerController* DSPlayerController = Cast<ADS_GamePlayerController>(Player);
	ADSGameState* DSGameState = GetGameState<ADSGameState>();
	if (IsValid(DSGameState) && IsValid(DSGameState->InGameState) && IsValid(DSPlayerController))
	{
		FDS_PlayerInfo PlayerInfo(DSPlayerController->Username);
		DSGameState->InGameState->AddPlayerInfo(PlayerInfo);
	}
}
void ADS_GameMode::RemovePlayerInfoFromLobbyState(AController* Player) const
{
	ADS_GamePlayerController* DSPlayerController = Cast<ADS_GamePlayerController>(Player);
	ADSGameState* DSGameState = GetGameState<ADSGameState>();
	if (IsValid(DSGameState) && IsValid(DSGameState->InGameState) && IsValid(DSPlayerController))
	{
		DSGameState->InGameState->RemovePlayerInfo(DSPlayerController->Username);
	}
}


