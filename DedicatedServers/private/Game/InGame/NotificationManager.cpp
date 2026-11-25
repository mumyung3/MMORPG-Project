// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/InGame/NotificationManager.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Player/IngameHUD.h"
#include "UI/Chat.h"
#include "UI/Main.h"
#include "Util/CustomLog.h"

// Sets default values
ANotificationManager::ANotificationManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	// 모든 클라이언트에서 관련성 유지
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ANotificationManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANotificationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANotificationManager::MulticastPlayerJoined_Implementation(const FString& PlayerName)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AHUD* HUD = PC->GetHUD();
		if (AIngameHUD* InGameHUD = Cast<AIngameHUD>(HUD))
		{
			if (!InGameHUD->GetMainUI())
			{
				LOGA("MAINUI가 존재하지않음.")
				return;
			}
			
			UChat* NewChat = CreateWidget<UChat>(GetWorld(), InGameHUD->GetMainUI()->Chat);
			if (NewChat)
			{
				NewChat->Username->SetText(FText::FromString(PlayerName));
				NewChat->ChatText->SetText(FText::Format(FText::FromString(TEXT("{0}님이 입장하였습니다.")), FText::FromString(PlayerName)
				));
                
				// ScrollBox에 추가
				InGameHUD->GetMainUI()->ScrollBox_0->AddChild(NewChat);
				InGameHUD->GetMainUI()->ScrollBox_0->ScrollToEnd();

			}
		}
	}
}
