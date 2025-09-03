// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/GameSession/JoinGameWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UJoinGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_GoBack->OnClicked.AddDynamic(this, &UJoinGameWidget::OnBackServerBrowser);
	//Button_Join->OnClicked.AddDynamic(this, &UJoinGameWidget::OnJoinGameSessionClicked);
}

void UJoinGameWidget::OnBackServerBrowser()
{
	OnGoBack.Broadcast();
}
void UJoinGameWidget::UpdateStatusMessage(const FString& StatusMessage, bool bResetJoinGameButton)
{

	check(IsValid(TextBlock_StatusMessage));
	TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
	check(IsValid(Button_Join));
	if (bResetJoinGameButton)
	{
		Button_Join->SetIsEnabled(true);
	}
	
}
//overlay에서 처리
// void UJoinGameWidget::OnJoinGameSessionClicked()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Join Server"));
// 	//이후 응답 실패 받으면 다시 true 성공 시, 트래블.
// 	Button_Join->SetIsEnabled(false);
// 	
//}