
#include "UI/UIComp.h"

#include "EnhancedInputComponent.h"
#include "Controllers/WarriorHeroController.h"
#include "Player/IngameHUD.h"
#include "UI/Main.h"

UUIComp::UUIComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUIComp::BeginPlay()
{
	Super::BeginPlay();
}

void UUIComp::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
// 실제 바인딩 되는 함수 이므로 꼭 super 및 오버라이딩 하기.

void UUIComp::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::SetupPlayerInputComponent(EnhancedInputComponent);
	EnhancedInputComponent->BindAction(IA_Enter, ETriggerEvent::Triggered, this, &UUIComp::IA_EnterAction);
}

void UUIComp::IA_EnterAction()
{
	PC->SetInputModeUIOnly(false);
	Cast<AIngameHUD>(PC->GetIngameHUD())->GetMainUI()->FocusChat();
}

void UUIComp::Server_SendMsg_Implementation(const FText& Text, const FText& Username)
{
	Multicast_SendMsg(Text, Username);
}

void UUIComp::Multicast_SendMsg_Implementation(const FText& Text, const FText& Username)
{
	// 서버 동작 x
	if (GetOwner()->HasAuthority())
	{
		return;
	}
	// 각 클라의 주인공에게
	if (auto TempPC = GetWorld()->GetFirstPlayerController())
	{
		// MainUI->ReceiveMsg(Text);
		Cast<AIngameHUD>(Cast<AWarriorHeroController>(TempPC)->GetIngameHUD())->GetMainUI()->ReceiveMsg(Text, Username);
	}
}
