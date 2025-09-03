// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SavepointMenuUI.h"
#include "Components/Button.h"
#include "Util/CustomLog.h"

void USavepointMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// ** 버튼마다 실행될 함수 등록 *
	ButtonToActionMap.Add(LvUpButton, [this]() { LOGC("LVUP") });
	ButtonToActionMap.Add(PotionButton, [this]() { LOGC("POTION") });
	ButtonToActionMap.Add(SaveButton, [this]() { LOGC("SAVE") });
	
	ButtonToActionMap.Add(ExitButton, [this]() { // 나가기 버튼 클릭 시 실행
		if (OnExitButton)
		{
			RemoveFromParent();
			OnExitButton();
		}
	});

	// ** 버튼 클릭 이벤트 바인딩 *
	LvUpButton->OnClicked.AddDynamic(this, &USavepointMenuUI::OnLvUpClicked);
	PotionButton->OnClicked.AddDynamic(this, &USavepointMenuUI::OnPotionClicked);
	SaveButton->OnClicked.AddDynamic(this, &USavepointMenuUI::OnSaveClicked);
	ExitButton->OnClicked.AddDynamic(this, &USavepointMenuUI::OnExitClicked);
}

void USavepointMenuUI::OnLvUpClicked() { ButtonToActionMap[LvUpButton](); }

void USavepointMenuUI::OnPotionClicked() { ButtonToActionMap[PotionButton](); }

void USavepointMenuUI::OnSaveClicked() { ButtonToActionMap[SaveButton](); }

void USavepointMenuUI::OnExitClicked() { ButtonToActionMap[ExitButton](); }