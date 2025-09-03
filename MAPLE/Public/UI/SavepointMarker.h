// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavepointMarker.generated.h"

/**
 * 
 */


UCLASS()
class MAPLE_API USavepointMarker : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* SavepointMarkerButton;

	FName SavepointID;

	class UWorldMapUI* OwningWorldMapUI; // 해당 세이브포인트 마커가 소속되어 있는 월드맵 UI 저장

	bool bIsClickEventBinded = false;

	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void OnSavepointMarkerClicked();

public:
	TFunction<void(USavepointMarker*)> OnClicked; // 마커가 클릭됐을 때 실행될 함수를 바인딩 (확인 창 띄우기)
	
	// ** Getter Function *
	FName GetSavepointID();
	
	// ** Setter Function *
	void SetSavepointID(FName InSavepointID);
	UFUNCTION()
	void SetOwningWorldMapUI(UWorldMapUI* OwnerMapUI);
};
