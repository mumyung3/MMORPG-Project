// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/IngameHUD.h"
#include "UI/Main.h"
#include "Util/CustomLog.h"

void AIngameHUD::BeginPlay()
{
	Super::BeginPlay();
	LOGA("IngameHUD BeginPlay 시작");
    
	OwnerPC = GetOwningPlayerController();
	
	if (MainUIClass)
	{
		MainUI = CreateWidget<UMain>(GetOwningPlayerController(), MainUIClass);
		if (MainUI)
		{
			MainUI->AddToViewport(0); // MainUI는 가장 맨 뒤에 띄운다.

			OnMainUIReady.Broadcast(MainUI);
		}
	}

	LOGA("MainUI 생성 완료: %s", GetMainUI() ? TEXT("Valid") : TEXT("NULL"));
}