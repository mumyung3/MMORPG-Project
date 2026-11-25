// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/NPC/VillagerNPC.h"

#include <DataAssets/NPC/NPCDialogDataAsset.h>

#include "Controllers/WarriorHeroController.h"
#include "Util/CustomLog.h"

AVillagerNPC::AVillagerNPC()
{
}

void AVillagerNPC::OnEnterInteract(APawn* Interactor)
{
	if (!Interactor)
		return;

	AWarriorHeroController* PC = Cast<AWarriorHeroController>(Interactor->GetController());
	if (!PC) return;
	
	PC->ShowNPCDialog(DialogData->NPCID);
}

void AVillagerNPC::OnInteract()
{
}

void AVillagerNPC::OnExitInteract()
{
}

FText AVillagerNPC::GetInteractDesc()
{
	return InteractDesc;
}

void AVillagerNPC::BeginPlay()
{
	Super::BeginPlay();

	InteractDesc = FText::FromString(TEXT("대화"));
}
