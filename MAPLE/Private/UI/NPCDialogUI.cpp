// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NPCDialogUI.h"
#include "Components/TextBlock.h"

void UNPCDialogUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetIsFocusable(true);
}

void UNPCDialogUI::NextButtonPressed()
{
	if (CurrentDialogLineIndex + 1 < DialogueLines.Num())
	{
		CurrentDialogLineIndex++;
		UpdateDialogText();
	}
	else
	{
		// 대사 종료
		bIsDialogLineEnded = true;
	}
}

void UNPCDialogUI::UpdateDialogText()
{
	if (DialogueLines.IsValidIndex(CurrentDialogLineIndex))
	{
		NPCDialogText->SetText(DialogueLines[CurrentDialogLineIndex]);
	}
}

void UNPCDialogUI::SetDialogData(const FText& InNPCName, const TArray<FText>& InNPCDialogLines)
{
	NPCNameText->SetText(InNPCName);
	
	if (NPCDialogText)
	{
		DialogueLines = InNPCDialogLines;
	}

	// Initial Setting
	CurrentDialogLineIndex = 0;
	bIsDialogLineEnded = false;

	UpdateDialogText(); // First Line Update
}

void UNPCDialogUI::HandleEscape()
{
	RemoveFromParent();

	OnDialogUIClosedEvent();
}

FReply UNPCDialogUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();

	if (PressedKey == EKeys::F || PressedKey == EKeys::SpaceBar)
	{
		NextButtonPressed();
			
		if (bIsDialogLineEnded) // NPC 대사 끝
		{
			HandleEscape();
		}
		
		return FReply::Handled();
	}
	else if (PressedKey == EKeys::Escape)
	{
		HandleEscape();
		return FReply::Handled(); // 키 입력 처리 완료
	}
	else
	{
		return FReply::Unhandled();
	}
}

FReply UNPCDialogUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		NextButtonPressed();
			
		if (bIsDialogLineEnded) // NPC 대사 끝
		{
			HandleEscape();
		}
		
		return FReply::Handled();
	}

	return FReply::Unhandled();
}