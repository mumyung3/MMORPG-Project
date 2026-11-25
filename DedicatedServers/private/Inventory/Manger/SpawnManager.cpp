// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Manger/SpawnManager.h"

#include "Character/WarriorHeroCharacter.h"
#include "Kismet/GameplayStatics.h"

void USpawnManager::StartTimer()
{
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &USpawnManager::SpawnActor, 30.0f, true);
	}
}

void USpawnManager::SpawnActor()
{
	SpawnActorsAroundAllPlayers(this, SpawnActors);
}

// 함수: 모든 플레이어 캐릭터 주변에 액터 생성
void USpawnManager::SpawnActorsAroundAllPlayers(UObject* WorldContextObject, const TArray<TSubclassOf<AActor>>& SpawnedActors)
{
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("WorldContextObject is null"));
        return;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("World is null"));
        return;
    }

    // 서버에서만 작업 수행
    if (!World->IsNetMode(NM_DedicatedServer))
    {
        UE_LOG(LogTemp, Warning, TEXT("This function should only be called on the server."));
        return;
    }

    // 모든 플레이어 캐릭터 가져오기
    TArray<AActor*> PlayerCharacters;
    UGameplayStatics::GetAllActorsOfClass(WorldContextObject, AWarriorHeroCharacter::StaticClass(), PlayerCharacters);

    // 각 플레이어 주변에 액터 스폰
    for (AActor* Actor : PlayerCharacters)
    {
    	if (AWarriorHeroCharacter* Player = Cast<AWarriorHeroCharacter>(Actor))
    	{
    		FVector PlayerLocation = Player->GetActorLocation();
    
    		int32 DropCount = FMath::RandRange(1, FMath::Min(5, SpawnedActors.Num()));
    		for (int32 i = 0; i < DropCount; ++i) {
    			int32 RandomIndex = FMath::RandRange(0, SpawnedActors.Num() - 1);
    			if (SpawnedActors[RandomIndex]) {
    				FVector Offset = FMath::VRand() * FMath::RandRange(200.0f, 500.0f);
    				Offset.Z = 30.0f;
    				FVector SpawnLocation = PlayerLocation + Offset;
    				World->SpawnActor<AActor>(SpawnedActors[RandomIndex], SpawnLocation, FRotator::ZeroRotator);
    			}
    		}
 
    	}
   }
}