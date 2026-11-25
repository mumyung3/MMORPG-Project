// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/GameEnums.h"
#include "IngameUIManager.generated.h"

struct FUIConfig
{
	TSubclassOf<UUserWidget> WidgetClass; // 위젯의 클래스
	TFunction<void(UUserWidget*)> OnCreateCallback = nullptr; // UI가 생성될 때 콜백되는 함수
	TFunction<void(UUserWidget*)> OnShowCallback = nullptr; // UI가 화면에 띄워질 때 전처리 과정 콜백

	int32 ZOrder = 0; // UI 우선순위
	bool bHideable = false; // UI를 setVisible로 show를 조절하는가?
	bool bNeedFocus = false; // UI를 띄울 때 Focus를 줘야하는가?
	bool bNeedCursorVisible = false; // UI를 띄울 때 커서가 보여야 하는가?
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMenuUIOpened, bool bIsOpened);

/**
 * 
 */
UCLASS()
class MAPLE_API UIngameUIManager : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<EUIType, UUserWidget*> UIMap; // 생성된 UI를 저장하는 맵
	TMap<EUIType, FUIConfig> UIConfigMap; // 생성된 UI의 Config를 저장하는 맵

	FText CachedDialogText; // 확인창에 띄울 메시지
	FName TargetSavepointID; // 텔레포트하려는 세이브포인트의 ID

	UPROPERTY()
	bool bIsUIOpened = false;

public:
	FOnMenuUIOpened OnMenuUIOpened; // 메뉴 UI가 열리게 되면 Broadcast하는 델리게이트
	
	// ** Initial Setting Function *
	void RegisterUI(EUIType UIType, const FUIConfig& Config);
	void RegisterAllUI(
		TSubclassOf<UUserWidget> InSavepointMenuUIClass,
		TSubclassOf<UUserWidget> InWorldMapUIClass,
		TSubclassOf<UUserWidget> InConfirmationDialogClass,
		TSubclassOf<UUserWidget> InNPCDialogUIClass,
		TSubclassOf<UUserWidget> InNPCMenuUIClass,
		TSubclassOf<UUserWidget> InNPCConversationUIClass
	);

	void CreateUI(EUIType UIType);
	void ShowUI(EUIType UIType);

	// ** UI Function *
	void UpdateMapSavepoint(class AIngamePlayerState* PS); // 세이브포인트를 발견하면 맵 업데이트
	void ShowConfirmationDialog(const FText& Message);
	void ExtractDialogData(class UNPCDialogDataAsset* DialogDataAsset, class UDialogManagerSubsystem* NPCConvSys); // NPC 대사 데이터에셋의 내용을 추출하여 NPCDialogUI에 전달.

	// ** Set Input Mode Function *
	TFunction<void()> SetInputModeGameOnly;
	TFunction<void(bool)> SetInputModeUIOnly;
	
	// ** Request Function *
	TFunction<void(FName)> RequestTeleportPlayerToSavepoint;
	TFunction<void(UUserWidget*, bool)> RequestWidgetFocus;

	// ** Set Function *
	UFUNCTION(BlueprintCallable)
	void SetMenuUIOpened(bool bOpened);

	// ** Get Function *
	bool IsUIOpened();
};
