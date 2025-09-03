// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InteractUI.h"
#include "Components/TextBlock.h"

void UInteractUI::InputKeySetting(FText InKey)
{
	InputKey->SetText(InKey);
}

void UInteractUI::DescriptionSetting(FText Desc)
{
	Description->SetText(Desc);
}
