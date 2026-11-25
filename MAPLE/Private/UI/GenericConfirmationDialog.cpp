// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GenericConfirmationDialog.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Util/CustomLog.h"

void UGenericConfirmationDialog::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (OKButton)
	{
		OKButton->OnClicked.AddDynamic(this, &UGenericConfirmationDialog::HandleOKButtonClicked);
	}

	if (NOButton)
	{
		NOButton->OnClicked.AddDynamic(this, &UGenericConfirmationDialog::HandleNOButtonClicked);
	}
}

void UGenericConfirmationDialog::HandleOKButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	
	if (OnOKClicked)
	{
		OnOKClicked();
	}
}

void UGenericConfirmationDialog::HandleNOButtonClicked()
{
	LOGC("NO BUTTON CLICKED")

	SetVisibility(ESlateVisibility::Collapsed);
}

void UGenericConfirmationDialog::SetDialogText(const FText& InMessage)
{
	if (DialogText)
	{
		DialogText->SetText(InMessage);
	}
}