
#include "UI/Main.h"

#include "Character/WarriorHeroCharacter.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Controllers/WarriorHeroController.h"
#include "Game/Shared/Player/DSLocalPlayerSubsystem.h"
#include "UI/Chat.h"
#include "UI/UIComp.h"
#include "UI/InteractUI.h"

void UMain::NativeConstruct()
{
	Super::NativeConstruct();

	InteractUI->SetVisibility(ESlateVisibility::Visible);
	EditableText_0->OnTextCommitted.AddDynamic(this, &UMain::CommitChat);
	
}
void UMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
void UMain::FocusChat()
{
	EditableText_0->SetKeyboardFocus();
}

void UMain::CommitChat(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (!Text.IsEmpty())
	{
		
		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
		FText Username = FText::FromString(LocalPlayerSubsystem->Username);
		// 네트워크
		SendMsg(Text, Username);
	}

	// INPUT 모드
	Cast<AWarriorHeroController>(GetOwningPlayer())->SetInputModeGameOnly();
	// TEXT 초기화
	EditableText_0->SetText(FText::GetEmpty());
}
void UMain::ReceiveMsg(const FText& Text, const FText& Username)
{
	UChat* NewChat = CreateWidget<UChat>(GetWorld(), Chat); //

	NewChat->Username->SetText(Username);
	NewChat->ChatText->SetText(Text);
	ScrollBox_0->AddChild(NewChat);
	ScrollBox_0->ScrollToEnd();
}
void UMain::SendMsg(const FText& Text, const FText& Username)
{
	Cast<AWarriorHeroCharacter>(GetOwningPlayer()->GetPawn())->GetUIComp()->Server_SendMsg(Text, Username);
}

void UMain::ShowInteractUI(bool bIsLookingObject)
{
	if (InteractUI)
	{
		if (bIsLookingObject) // UI ON
		{
			InteractUI->SetVisibility(ESlateVisibility::Visible);
		}
		else // UI OFF
		{
			InteractUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UMain::SetInteractText(FText Desc)
{
	InteractUI->DescriptionSetting(Desc);
}

void UMain::SetInteractInputKey(FText InKey)
{
	InteractUI->InputKeySetting(InKey);
}

UDSLocalPlayerSubsystem* UMain::GetDSLocalPlayerSubsystem() const
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(LocalPlayerController->Player);
		if (IsValid(LocalPlayer))
		{
			return LocalPlayer->GetSubsystem<UDSLocalPlayerSubsystem>();
		}
	}
	return nullptr;
}
