// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/EscClosableUI.h"
#include "WorldMapUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapClosedDelegate);

/**
 *
 */
UCLASS()
class MAPLE_API UWorldMapUI : public UUserWidget, public IEscClosableUI
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UOverlay* WorldMapOverlay;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* WorldMapImage;
	
	FVector2D MapPos;

	float ZoomFactor = 1.0f; // 현재 줌 배율
	const float ZoomMin = 0.5f; // 줌 최소 배율
	const float ZoomMax = 3.0f; // 줌 최대 배율
	const float ZoomSensitivity = 0.1f; // 줌 감도 (작을수록 느림)

	bool bSavepointMarkerCreated = false; // 세이브포인트 마커가 생성되었는지 확인용 플래그
	bool bIsLMBPressed = false;

	// ** Savepoint Marker *
	UPROPERTY(EditAnywhere, Category = "Savepoint Marker UI")
	TSubclassOf<UUserWidget> SavepointMarkerUIClass;
	UPROPERTY()
	TArray<FName> AlreadyVisibleSavepoints; // Visible로 설정된 세이브포인트 마커가 저장됨 (함수 중복 실행 방지용)
	UPROPERTY(EditAnywhere, Category = "Savepoint Marker UI", meta = (AllowPrivateAccess = "true"))
	class USavepointDataAsset* SavepointDataAsset; // 세이브포인트 데이터 에셋
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* MarkerPanel; // 마커를 그릴 패널(레이어)

	UPROPERTY()
	TMap<FName, class USavepointMarker*> SavepointMarkerMap; // 세이브포인트 ID : 세이브포인트 마커 위젯 맵

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* MapOpenSound;

public:
	void InitializeSetting(); // 초기 설정 (함수 실행 순서때문에 생성하였음)

	// ** Savepoint Marker Function *
	void UpdateSavepointMarkerPos(); // 월드맵을 움직일 시 맵 상의 세이브포인트 마커의 위치를 갱신하는 함수
	void CreateSavepointMarker(); // 세이브포인트 마커 생성
	void ShowDiscoveredSavepoints(class AIngamePlayerState* PS); // 발견한 세이브포인트를 띄움.
	void ShowSavepointMarkerIfFound(FName SavepointID); // 발견한 세이브포인트를 Visible로 설정

	void ZoomMap(float DeltaZoom);

	virtual void HandleEscape() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	TFunction<void()> OnMapClosedEvent; // 맵을 닫을 때 실행되는 콜백
	TFunction<void(FName)> SetTeleportSavepointID; // 텔레포트할 세이브포인트를 넘겨주기 위한 콜백
	TFunction<void(FText)> RequestShowGenericConfirmationDialog; // 텔레포트할 때 확인창을 띄우기 위한 요청 콜백
};