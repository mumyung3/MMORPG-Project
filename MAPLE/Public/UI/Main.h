// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/Shared/Player/DSLocalPlayerSubsystem.h"
#include "Main.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API UMain : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* EditableText_0;

	UPROPERTY(meta = (BindWidget))
	class UInteractUI* InteractUI;

	void FocusChat();
	// 실제로 블프에서 전달함.
	UFUNCTION(BlueprintCallable, Category = "Chat")
	virtual void CommitChat(const FText& Text, ETextCommit::Type CommitMethod);
	// 블루 프린트 삽입
	UPROPERTY(EditDefaultsOnly, Category = "Chat")
	TSubclassOf<UUserWidget> Chat;

	void ReceiveMsg(const FText& Text, const FText& Username);
	void SendMsg(const FText& Text, const FText& Username);

	void ShowInteractUI(bool bIsLookingObject);
	void SetInteractText(FText Desc);
	void SetInteractInputKey(FText InKey);
	UDSLocalPlayerSubsystem* GetDSLocalPlayerSubsystem()const;
};
