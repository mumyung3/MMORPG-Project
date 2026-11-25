// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/MainMenu/Configure/ConfigureBrowser.h"
#include "Components/Button.h"

void UConfigureBrowser::NativeConstruct()
{
	Super::NativeConstruct();

	check(IsValid(Button_GoBack));
	Button_GoBack->OnClicked.AddDynamic(this, &UConfigureBrowser::GoBackWidget);
}

void UConfigureBrowser::GoBackWidget()
{
	OnGoBack.Broadcast();
	
}