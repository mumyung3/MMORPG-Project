// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "Game/InGame/DS_GamePlayerController.h"
#include "WarriorHeroController.generated.h"

class URespawnComponent;
class UWarriorWidgetBase;
/**
 * 
 */

UCLASS()
class MAPLE_API AWarriorHeroController : public ADS_GamePlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AWarriorHeroController();

	//~ Begin IGenericTeamAgentInterface Interface
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface

	//GASD
	void CreateHUD();

	UWarriorWidgetBase* GetHUD();

	// NPC Dialog DataAsset List
	UPROPERTY(EditDefaultsOnly)
	TArray<class UNPCDialogDataAsset*> NPCDialogDataAssetList;

	// ** UI Class *
	UPROPERTY(EditAnywhere, Category = "Generic Question Box UI")
	TSubclassOf<UUserWidget> GenericConfirmationDialogUIClass;

	UPROPERTY(EditAnywhere, Category = "Savepoint Menu UI")
	TSubclassOf<UUserWidget> SavepointMenuUIClass;

	UPROPERTY(EditAnywhere, Category = "WorldMap UI")
	TSubclassOf<UUserWidget> WorldMapUIClass;

	UPROPERTY(EditAnywhere, Category = "NPC Dialog UI")
	TSubclassOf<UUserWidget> NPCDialogUIClass;

	UPROPERTY(EditAnywhere, Category = "NPC Menu UI")
	TSubclassOf<UUserWidget> NPCMenuUIClass;

	UPROPERTY(EditAnywhere, Category = "NPC Conversation UI")
	TSubclassOf<UUserWidget> NPCConversationUIClass;

	// ** Show UI Function *
	void ShowMainMenu(); // Main Menu UI
	void ShowSavepointMenu(); // Savepoint Menu UI 공용 함수
	UFUNCTION(Client, Reliable)
	void ClientShowSavepointMenu(); // 세이브포인트와 상호작용한 클라이언트에게 UI를 띄우라고 호출
	void ShowWorldMap();
	void ShowNPCDialog(FName InNPCID);
	UFUNCTION(Client, Reliable)
	void ClientShowNPCDialog(FName InNPCID); // NPC와 상호작용한 클라이언트에게 UI를 띄우라고 호출

	// ** World Map Function *
	void UpdateSavepointMarkerVisibility(); // 플레이어가 세이브포인트를 새로 발견했다면 호출됨

	// ** Savepoint Teleport Function *
	UFUNCTION(Server, Reliable)
	void ServerTeleportPlayerToSavepoint(FName SavepointID); // 서버에게 플레이어를 텔레포트시켜달라고 요청
	
	void TeleportPlayerToSavepoint(FName SavepointID); // 플레이어를 세이브포인트로 텔레포트시킴 (서버 실행)

	// ** NPC Conversation Function *
	UFUNCTION(BlueprintCallable)
	void NPCConversationEnd();
	
	// ** UIManager Initial Setting *
	void InitUIManager();

	// ** Input Function *
	void Interact();
	void LockPick();

	// ** Setter Function *
	void SetInputModeGameOnly();
	void SetInputModeUIOnly(bool bCursorVisible);
	
	// ** Getter Function *
	UFUNCTION(BlueprintCallable)
	class UIngameUIManager* GetUIManager() { return IngameUIManager; }
	bool IsMenuUIOpened();
	class AIngameHUD* GetIngameHUD();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GASD|UI")
	TSubclassOf<UWarriorWidgetBase> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "GASD|UI")
	UWarriorWidgetBase* UIHUDWidget;

	// ** Input Actions *
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") // 월드맵 키
	class UInputAction* IA_WorldMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") // ESC 키
	UInputAction* IA_Escape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") // 상호작용 키
	UInputAction* IA_Interact;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") // 락픽 키
	UInputAction* IA_LockPick;
	
	UPROPERTY(EditAnywhere, Category = "Savepoint Marker UI", meta = (AllowPrivateAccess = "true"))
	class USavepointDataAsset* SavepointDataAsset; // 세이브포인트 데이터 에셋

	// Server Only
	virtual void OnPossess(APawn* aPawn) override;

	// Client Only
	virtual void SetPawn(APawn* InPawn) override;
	
	virtual void OnRep_PlayerState() override;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	FGenericTeamId HeroTeamID;
	
	FTimerHandle ComponentCacheTimerHandle;
	
	UPROPERTY() // NPC Dialog DataAsset Map
	TMap<FName, UNPCDialogDataAsset*> NPCDialogDataAssetMap;

	UPROPERTY()
	class UIngameUIManager* IngameUIManager;

	// ** Cached Components *
	UPROPERTY()
	class UInteractInputComp* InteractionComponent;

	UPROPERTY()
	class ULockPickInputComp* LockPickComponent;

	void CachePawnComponents(); // Cache Components
	
	void BuildNPCDialogMap(); // NPC Dialog Map Create Function

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Savepoint Marker UI")
	URespawnComponent* RespawnComponent;
	
};
