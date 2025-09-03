// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/BaseTrap.h"
#include "Components/BoxComponent.h"

ABaseTrap::ABaseTrap()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	RootComponent = TriggerArea;
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ABaseTrap::OnTriggerBoxBeginOverlap);

	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
	TrapMesh->SetupAttachment(RootComponent);
}

void ABaseTrap::TriggerTrap()
{
	// 자식에서 함수 구현
}

void ABaseTrap::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseTrap::OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerTrap();
}
