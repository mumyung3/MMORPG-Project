// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WarriorWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"

void UWarriorWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn())) {

		checkf(PawnUIInterface, TEXT("WarriorWidgetBase / NativeOnInitialized / PawnUIInterface"))

		if (UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent()) {
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
			UE_LOG(LogTemp, Warning, TEXT("WarriorWidgetBase / NativeOnInitialized / Successed"));
		}
	}
}

void UWarriorWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor)) {
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();

		BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);

		UE_LOG(LogTemp, Warning, TEXT("WarriorWidgetBase / InitEnemyCreatedWidget / Successed"));
	}
}
