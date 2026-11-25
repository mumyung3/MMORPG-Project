// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WarriorHeroController.h"

#include <DataAssets/NPC/NPCDialogDataAsset.h>

#include "EnhancedInputComponent.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/Combat/RespawnComponent.h"
#include "DataAssets/WorldMap/SavepointDataAsset.h"
#include "Input/InteractInputComp.h"
#include "Input/LockPickInputComp.h"
#include "LLMDialogSystem/DialogManagerSubsystem.h"
#include "Player/IngameHUD.h"
#include "Player/IngamePlayerState.h"
#include "Types/GameEnums.h"
#include "UI/Manager/IngameUIManager.h"
#include "Util/CustomLog.h"
#include "Widgets/WarriorWidgetBase.h"

AWarriorHeroController::AWarriorHeroController()
{
	HeroTeamID = FGenericTeamId(0);

	RespawnComponent = CreateDefaultSubobject<URespawnComponent>(TEXT("RespawnComponent"));

}

FGenericTeamId AWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}

void AWarriorHeroController::CreateHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("Call CreateHUD"));
	// Only create once
	if (UIHUDWidget)
	{
		return;
	}
	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}
	if (!IsLocalPlayerController())
	{
		return;
	}
	AIngamePlayerState* PS = GetPlayerState<AIngamePlayerState>();
	if (!PS)
	{
		return;
	}
	UIHUDWidget = CreateWidget<UWarriorWidgetBase>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();

	UIHUDWidget->SetCurrentHealth(PS->GetCurrentHealth());
	UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	UIHUDWidget->SetHealthPercentage(PS->GetCurrentHealth() / FMath::Max<float>(PS->GetMaxHealth(), 1.f));
	UIHUDWidget->SetHealthRegenRate(PS->GetHealthRegenRate());
	UIHUDWidget->SetCurrentStamina(PS->GetCurrentStamina());
	UIHUDWidget->SetMaxStamina(PS->GetMaxStamina());
	UIHUDWidget->SetStaminaPercentage(PS->GetCurrentStamina() / FMath::Max<float>(PS->GetMaxStamina(), 1.f));
	UIHUDWidget->SetStaminaRegenRate(PS->GetStaminaRegenRate());
}

void AWarriorHeroController::ShowMainMenu()
{
	IngameUIManager->ShowUI(EUIType::MainMenu);
}

void AWarriorHeroController::ShowSavepointMenu()
{
	if (IsLocalController())
	{
		ClientShowSavepointMenu(); // 이미 로컬이면 직접 호출
	}
	else
	{
		ClientShowSavepointMenu();
	}
}

void AWarriorHeroController::ClientShowSavepointMenu_Implementation()
{
	IngameUIManager->ShowUI(EUIType::SavePointMenu);
}

void AWarriorHeroController::ShowWorldMap()
{
	if (IngameUIManager)
		IngameUIManager->ShowUI(EUIType::WorldMap);
}

void AWarriorHeroController::ShowNPCDialog(FName InNPCID)
{
	if (IsLocalController())
	{
		ClientShowNPCDialog(InNPCID); // 이미 로컬이면 직접 호출
	}
	else
	{
		ClientShowNPCDialog(InNPCID);
	}
}

void AWarriorHeroController::ClientShowNPCDialog_Implementation(FName InNPCID)
{
	auto DialogData = NPCDialogDataAssetMap[InNPCID];
	auto DialogManagerSubsystem = GetGameInstance()->GetSubsystem<UDialogManagerSubsystem>();
	
	IngameUIManager->ExtractDialogData(DialogData, DialogManagerSubsystem);
	
	IngameUIManager->ShowUI(EUIType::NPCDialog);
}

void AWarriorHeroController::UpdateSavepointMarkerVisibility()
{
	if (AIngamePlayerState* PS = GetPlayerState<AIngamePlayerState>())
	{
		IngameUIManager->UpdateMapSavepoint(PS);
	}
}

void AWarriorHeroController::ServerTeleportPlayerToSavepoint_Implementation(FName SavepointID)
{
	TeleportPlayerToSavepoint(SavepointID);
}

void AWarriorHeroController::TeleportPlayerToSavepoint(FName SavepointID)
{
	if (const FSavepointData* Data = SavepointDataAsset->FindDataByID(SavepointID))
	{
		if (APawn* PlayerPawn = GetPawn())
		{
			PlayerPawn->SetActorLocation(Data->WorldPosition + FVector(100.f, 0.f, 50.f));
		}
	}
}

void AWarriorHeroController::NPCConversationEnd()
{
	auto DialogManagerSubsystem = GetGameInstance()->GetSubsystem<UDialogManagerSubsystem>();
	DialogManagerSubsystem->ClearDialogHistory();
}

void AWarriorHeroController::InitUIManager()
{
	// UIManager InputMode 설정 함수 바인딩
	IngameUIManager->SetInputModeGameOnly = [this](){ SetInputModeGameOnly(); };
	IngameUIManager->SetInputModeUIOnly = [this](bool bCursor){ SetInputModeUIOnly(bCursor); };

	// UI 함수 바인딩
	IngameUIManager->RequestTeleportPlayerToSavepoint = [this](FName SavepointID) {
		ServerTeleportPlayerToSavepoint(SavepointID);
	};
	IngameUIManager->RequestWidgetFocus = [this](UUserWidget* Widget, bool bCursor) {
		if (!IsValid(Widget)) return;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(Widget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		SetInputMode(InputMode);
		bShowMouseCursor = bCursor;
	};

	// UI 등록
	IngameUIManager->RegisterAllUI(
		SavepointMenuUIClass,
		WorldMapUIClass,
		GenericConfirmationDialogUIClass,
		NPCDialogUIClass,
		NPCMenuUIClass,
		NPCConversationUIClass);
}

void AWarriorHeroController::Interact()
{
	if (!IsMenuUIOpened())
		InteractionComponent->BeginInteraction();
}

void AWarriorHeroController::LockPick()
{
	if (!IsMenuUIOpened())
		LockPickComponent->LockPick();
}

void AWarriorHeroController::SetInputModeGameOnly()
{
	FInputModeGameOnly InputModeGame;
	SetInputMode(InputModeGame);
	bShowMouseCursor = false;
}

void AWarriorHeroController::SetInputModeUIOnly(bool bCursorVisible)
{
	FInputModeUIOnly InputModeUI;
	SetInputMode(InputModeUI);
	bShowMouseCursor = bCursorVisible;
}

bool AWarriorHeroController::IsMenuUIOpened()
{
	return (IngameUIManager && IngameUIManager->IsUIOpened());
}

AIngameHUD* AWarriorHeroController::GetIngameHUD()
{
	return Cast<AIngameHUD>(this->MyHUD);
}

class UWarriorWidgetBase* AWarriorHeroController::GetHUD()
{
	return UIHUDWidget;
}

void AWarriorHeroController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AIngamePlayerState* PS = GetPlayerState<AIngamePlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, aPawn);
	}
}

void AWarriorHeroController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (IsLocalController() && InPawn)
	{
		// 기존 타이머 있으면 초기화
		GetWorld()->GetTimerManager().ClearTimer(ComponentCacheTimerHandle);

		// 0.1초마다 캐싱 시도
		GetWorld()->GetTimerManager().SetTimer(ComponentCacheTimerHandle, this, &AWarriorHeroController::CachePawnComponents, 0.1f, true);
	}
}

void AWarriorHeroController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//CreateHUD();
}

void AWarriorHeroController::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()	&& !IngameUIManager)
	{
		// UIManager 생성
		IngameUIManager = NewObject<UIngameUIManager>(this); // Outer는 PC로 설정
	}

	if (IngameUIManager) // 서버 실행 X
	{
		InitUIManager();

		// UI 생성
		IngameUIManager->CreateUI(EUIType::SavePointMenu);
		IngameUIManager->CreateUI(EUIType::WorldMap);
		IngameUIManager->CreateUI(EUIType::ConfirmationDialog);
		IngameUIManager->CreateUI(EUIType::NPCDialog);
		IngameUIManager->CreateUI(EUIType::NPCMenu);
		IngameUIManager->CreateUI(EUIType::NPCConversation);
	}

	BuildNPCDialogMap();
}

void AWarriorHeroController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(IA_WorldMap, ETriggerEvent::Started, this, &AWarriorHeroController::ShowWorldMap); // 월드맵 키 바인딩
		EnhancedInput->BindAction(IA_Escape, ETriggerEvent::Started, this, &AWarriorHeroController::ShowMainMenu); // ESC 키 바인딩
		EnhancedInput->BindAction(IA_Interact, ETriggerEvent::Started, this, &AWarriorHeroController::Interact); // 상호작용 키 바인딩
		EnhancedInput->BindAction(IA_LockPick, ETriggerEvent::Started, this, &AWarriorHeroController::LockPick); // 락픽 키 바인딩
	}
}

void AWarriorHeroController::CachePawnComponents()
{
	auto HeroCharacter = Cast<AWarriorHeroCharacter>(GetPawn());

	if (!HeroCharacter) return;
	
	if (HeroCharacter->GetInteractInputComp() && HeroCharacter->GetLockPickInputComp())
	{
		InteractionComponent = HeroCharacter->GetInteractInputComp();
		LockPickComponent = HeroCharacter->GetLockPickInputComp();


		if (IngameUIManager)
		{
			IngameUIManager->OnMenuUIOpened.AddUObject(InteractionComponent, &UInteractInputComp::OnUIOpened);

			GetWorld()->GetTimerManager().ClearTimer(ComponentCacheTimerHandle); // 캐싱 완료 후 타이머 정지
			LOGC("CACHE COMPLETE!")
		}
	}

}

void AWarriorHeroController::BuildNPCDialogMap()
{
	NPCDialogDataAssetMap.Empty();

	for (UNPCDialogDataAsset* Asset : NPCDialogDataAssetList)
	{
		if (Asset && Asset->NPCID != NAME_None)
		{
			NPCDialogDataAssetMap.Add(Asset->NPCID, Asset);
		}
	}
}
