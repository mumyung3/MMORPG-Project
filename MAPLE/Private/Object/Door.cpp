// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/Door.h"

#include "Character/WarriorHeroCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Util/CustomLog.h"
#include "Kismet/GameplayStatics.h"
#include "Player/InventoryComp.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsOpen = false;
	bIsLocked = true;

	DoorID = "DOOR";

	// DoorFrameMesh
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMesh->SetupAttachment(RootComponent);

	// DoorMesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepRelativeTransform);

	DoorMesh->SetRelativeLocation(FVector(0, 45.0f, 0));

	// Box Collision
	PlayerDetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerDetectBox"));
	PlayerDetectBox->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepRelativeTransform);

	PlayerDetectBox->SetRelativeLocation(FVector(0, 0, 120));
	PlayerDetectBox->SetBoxExtent(FVector(160, 120, 150));

	// Door Open Timeline
	DoorOpenTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorOpenTimeline"));
	DoorOpenTimeline->SetTimelineLength(0.5f);
	DoorOpenTimeline->SetLooping(false);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap function
	PlayerDetectBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::BeginOverlap);
	PlayerDetectBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::EndOverlap);

	// Door Open Timeline
	DoorOpenTimelineUpdateDeligate.BindUFunction(this, FName("UpdateDoorTimeline"));
	DoorOpenTimeline->AddInterpFloat(DoorOpenTimelineCurve, DoorOpenTimelineUpdateDeligate);

	MoveDoor(!bIsOpen);

	if (bIsLocked)
	{
		InteractInputKey = FText::FromString(TEXT("C"));
		InteractDesc = FText::FromString(TEXT("문 따기"));
	}
	else
	{
		InteractDesc = FText::FromString(TEXT("열기"));
	}
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bIsOpen);
	DOREPLIFETIME(ADoor, bIsLocked);
}

void ADoor::OnRep_DoorOpen()
{
	MoveDoor(!bIsOpen);
}

void ADoor::OnRep_SetDoorUnlocked()
{
	SetDoorUnlocked();
}

void ADoor::Multicast_PlayOpenSound_Implementation()
{
	if (DoorOpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			DoorOpenSound,
			GetActorLocation(),
			1.0f,
			FMath::FRandRange(0.9f, 1.1f),
			0.0f,
			AttenuationSettings
		);
	}
}

void ADoor::Multicast_PlayUnlockSound_Implementation()
{
	if (DoorUnlockSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			DoorUnlockSound,
			GetActorLocation(),
			1.0f,
			FMath::FRandRange(0.9f, 1.1f),
			0.0f,
			AttenuationSettings
		);
	}
}

bool ADoor::CheckKey()
{
	AWarriorHeroCharacter* Character = Cast<AWarriorHeroCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	UInventoryComp* inv = Character->GetInventoryComp();

	if (Character && inv->HasItem(RequiredKey))
	{
		return true;
	}
	else
		return false;
}

bool ADoor::IsDoorLocked()
{
	if (bIsLocked)
		return true;
	else
		return false;
}

void ADoor::SetDoorUnlocked()
{
	bIsLocked = false;
	InteractInputKey = FText::FromString(TEXT("F"));
	InteractDesc = FText::FromString(TEXT("열기"));
	Multicast_PlayUnlockSound();
	LOGC("DOOR UNLOCKED!")
}

void ADoor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	this->EnableInput(GetWorld()->GetFirstPlayerController());
}

void ADoor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	this->DisableInput(GetWorld()->GetFirstPlayerController());
}

void ADoor::UpdateDoorTimeline(float Output)
{
	DoorLerp = UKismetMathLibrary::RLerp(FRotator(0, 0, 0), FRotator(0, -90, 0), Output, true);
	DoorMesh->SetRelativeRotation(DoorLerp);
}

void ADoor::DoorOpen()
{
	if (bIsLocked) // Door is Locked
	{
		if (CheckKey()) // Player has RequiredKey
		{
			bIsLocked = false;
		}
	}
	else // Door is Unlocked
	{
		MoveDoor(bIsOpen);
		Multicast_PlayOpenSound();
		bIsOpen = !bIsOpen; // bIsOpen Value Changed -> OnRep_DoorOpen() CallBack.
	}
}

void ADoor::MoveDoor(bool bDoorState)
{
	if (bDoorState) // Door is Open -> Close the Door
	{
		DoorOpenTimeline->Reverse();
		InteractDesc = FText::FromString(TEXT("열기"));
	}
	else // Door is Closed -> Open the Door
	{
		DoorOpenTimeline->Play();
		InteractDesc = FText::FromString(TEXT("닫기"));
	}
}

void ADoor::OnEnterInteract(APawn* Interactor)
{
	DoorOpen();
}

void ADoor::OnInteract()
{
	LOGC("KEEP")
}

void ADoor::OnExitInteract(){
	LOGC("EXIT")
}

FText ADoor::GetInteractDesc()
{
	return InteractDesc;
}