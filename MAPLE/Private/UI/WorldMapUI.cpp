// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WorldMapUI.h"

#include "Components/Image.h"
#include "Util/CustomLog.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "DataAssets/WorldMap/SavepointDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Player/IngamePlayerState.h"
#include "UI/SavepointMarker.h"

void UWorldMapUI::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	MapPos = WorldMapImage->GetRenderTransform().Translation;

	if (MapOpenSound)
		UGameplayStatics::PlaySound2D(this, MapOpenSound);
}

void UWorldMapUI::InitializeSetting()
{
	CreateSavepointMarker();
}

void UWorldMapUI::UpdateSavepointMarkerPos()
{
	for (auto& Pair : SavepointMarkerMap)
	{
		USavepointMarker* Marker = Pair.Value;
		if (!Marker) continue;

		FSavepointData* SavepointData = SavepointDataAsset->FindDataByID(Pair.Key);
		if (!SavepointData) continue;

		UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(Marker->Slot);
		if (!MarkerSlot) continue;
		
		// NormalizedPos: (0.0 ~ 1.0) 사이 값
		FVector2D Pos = SavepointData->MapPosition;

		// ZoomFactor 반영
		FVector2D ZoomedPos = Pos * ZoomFactor;

		// 월드맵 현재 이동 적용 (NewTranslation)
		FVector2D FinalPos = ZoomedPos + MapPos;

		// 마커 위치 갱신
		MarkerSlot->SetPosition(FinalPos);
	}
}

void UWorldMapUI::CreateSavepointMarker()
{
	if (!SavepointDataAsset) return;
	
	for (const FSavepointData& SavepointData : SavepointDataAsset->SavepointList) // 세이브포인트 데이터 에셋 순회
	{
		USavepointMarker* SavepointMarker = CreateWidget<USavepointMarker>(this, SavepointMarkerUIClass);
		SavepointMarker->SetOwningWorldMapUI(this); // 소속 월드맵 UI 설정
		SavepointMarker->SetSavepointID(SavepointData.SavepointID); // 해당 세이브포인트 마커에 세이브포인트 ID 저장

		SavepointMarker->OnClicked = [this](USavepointMarker* ClickedMarker) { // 세이브포인트 마커가 클릭되었을 때 실행될 함수를 바인딩
			SetTeleportSavepointID(ClickedMarker->GetSavepointID());
			RequestShowGenericConfirmationDialog(FText::FromString(TEXT("해당 위치로 이동하시겠습니까?")));
			// 차라리 여기에다 플래그를 넣어서 showgeneric...함수에서 이를 구분하게 하면 되지 않을까?
		};
		
		if (!SavepointMarker) continue;
		
		MarkerPanel->AddChild(SavepointMarker); // 세이브포인트 마커를 일단 추가해놓고 위치 조정
		
		if (UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(SavepointMarker->Slot)) // 세이브포인트 마커 위치 적용
		{
			MarkerSlot->SetAnchors(FAnchors(0.5f, 0.5f)); // 앵커를 전체 화면으로 설정
			
			MarkerSlot->SetAlignment(FVector2D(0.5f, 0.5f)); // 가운데 정렬

			MarkerSlot->SetAutoSize(true); // Size To Content ON
			
			MarkerSlot->SetPosition(SavepointData.MapPosition);
		}
        
		SavepointMarker->SetVisibility(ESlateVisibility::Collapsed);
		
		SavepointMarkerMap.Add(SavepointData.SavepointID, SavepointMarker);
	}
}

void UWorldMapUI::ShowDiscoveredSavepoints(AIngamePlayerState* PS)
{
	if (!PS) return;
	
	for (auto& SavepointID : PS->GetDiscoveredSavepoints())
	{
		if (!AlreadyVisibleSavepoints.Contains(SavepointID)) // 중복 실행 방지용 함수
		{
			ShowSavepointMarkerIfFound(SavepointID);
			AlreadyVisibleSavepoints.Add(SavepointID);
		}
	}
}

void UWorldMapUI::ShowSavepointMarkerIfFound(FName SavepointID)
{
	USavepointMarker** FoundSavepointMarker = SavepointMarkerMap.Find(SavepointID);
	if (FoundSavepointMarker)
	{
		(*FoundSavepointMarker)->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWorldMapUI::ZoomMap(float DeltaZoom)
{
	// 기존 줌 값 저장
	float OldZoom = ZoomFactor;

	// 새로운 줌 값 계산
	ZoomFactor = FMath::Clamp(ZoomFactor + DeltaZoom * ZoomSensitivity, ZoomMin, ZoomMax);
	float ScaleDelta = ZoomFactor / OldZoom;

	// 기존 Translation
	FVector2D OldTranslation = WorldMapImage->GetRenderTransform().Translation;

	// 위젯 크기
	FVector2D WidgetSize = WorldMapImage->GetDesiredSize();

	// 현재 화면 중심 좌표 (현재 확대/이동 상태 기준)
	FVector2D ViewCenter = (WidgetSize * 0.5f) * OldZoom + OldTranslation;

	// Scale 변화에 따라 중심 보정
	FVector2D NewViewCenter = ViewCenter * ScaleDelta;

	// 새로운 Translation 계산
	FVector2D NewTranslation = NewViewCenter - (WidgetSize * 0.5f) * ZoomFactor;

	// 월드맵 좌표에 저장
	MapPos = NewTranslation;

	// 적용
	FWidgetTransform NewTransform;
	NewTransform.Scale = FVector2D(ZoomFactor);
	NewTransform.Translation = NewTranslation;
	WorldMapImage->SetRenderTransform(NewTransform);

	UpdateSavepointMarkerPos();
}

void UWorldMapUI::HandleEscape()
{
	RemoveFromParent();

	OnMapClosedEvent();

	if (MapOpenSound)
		UGameplayStatics::PlaySound2D(this, MapOpenSound);
}

FReply UWorldMapUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();

	if (PressedKey == EKeys::Escape || PressedKey == EKeys::Tab)
	{
		HandleEscape();

		return FReply::Handled(); // 키 입력 처리 완료
	}
	
	return FReply::Unhandled();
}

FReply UWorldMapUI::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	float ScrollDelta = InMouseEvent.GetWheelDelta(); // ±1 값
	
	ZoomMap(ScrollDelta);

	return FReply::Handled();
}

FReply UWorldMapUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsLMBPressed = true;
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply UWorldMapUI::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsLMBPressed = false;
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply UWorldMapUI::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;

	if (bIsLMBPressed)
	{
		MapPos += InMouseEvent.GetCursorDelta();

		WorldMapImage->SetRenderTranslation(MapPos);

		UpdateSavepointMarkerPos();

		return FReply::Handled();
	}

	return FReply::Unhandled();
}