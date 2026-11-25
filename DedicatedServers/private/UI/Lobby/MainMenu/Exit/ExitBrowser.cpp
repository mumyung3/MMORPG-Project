// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/MainMenu/Exit/ExitBrowser.h"
#include "Components/Button.h"
#include "Game/Shared/Player/DSLocalPlayerSubsystem.h"
#include "HTTP/Login/LoginManager.h"

void UExitBrowser::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Exit->OnClicked.AddDynamic(this, &UExitBrowser::OnExit);
	Button_GoBack->OnClicked.AddDynamic(this, &UExitBrowser::OnGoBackWidget);
}
/**
 *
 * @todo 종료 호출 시 , 로그아웃 연결하기
 */
void UExitBrowser::OnExit()
{
	// 게임 종료 호출
	//UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
	// login manager를 다시 불러오기에 mainmenuoverlay를 불러오기엔 좀 그러므로 localsubsystem으로 받아옴.
	TObjectPtr<ULoginManager> LoginManger = Cast<ULoginManager>(GetLocalPlayerSubsystem()->LoginManagementInterface.GetObject());
	check(LoginManger);
	LoginManger->SignOut(GetLocalPlayerSubsystem()->GetAuthResult().AccessToken);
	
}
void UExitBrowser::OnGoBackWidget()
{
	OnGoBack.Broadcast();
}
UDSLocalPlayerSubsystem* UExitBrowser::GetLocalPlayerSubsystem(){
	
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) && IsValid(PlayerController->GetLocalPlayer()))
	{
		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UDSLocalPlayerSubsystem>();
		if (IsValid(LocalPlayerSubsystem))
		{
			return LocalPlayerSubsystem;
		}
	}
	return nullptr;
	
}