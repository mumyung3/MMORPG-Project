// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SavepointMarker.h"

#include "Components/Button.h"
#include "UI/WorldMapUI.h"
#include "Util/CustomLog.h"

void USavepointMarker::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (SavepointMarkerButton && !bIsClickEventBinded)
	{
		SavepointMarkerButton->OnClicked.AddDynamic(this, &USavepointMarker::OnSavepointMarkerClicked);
		bIsClickEventBinded = true;
	}
}

void USavepointMarker::OnSavepointMarkerClicked()
{
	if (OnClicked)
	{
		OnClicked(this);
	}
}

FName USavepointMarker::GetSavepointID()
{
	return SavepointID;
}

void USavepointMarker::SetSavepointID(FName InSavepointID)
{
	SavepointID = InSavepointID;
}

void USavepointMarker::SetOwningWorldMapUI(UWorldMapUI* OwnerMapUI)
{
	OwningWorldMapUI = OwnerMapUI;
}