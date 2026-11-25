// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Chat.generated.h"

/**
 * 
 */
UCLASS()
class MAPLE_API UChat : public UUserWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(meta= (BindWidget))
	class UTextBlock* Username;
	
	UPROPERTY(meta= (BindWidget))
	class UTextBlock* ChatText;

};
