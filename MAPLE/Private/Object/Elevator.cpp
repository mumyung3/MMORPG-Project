// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/Elevator.h"
#include "Components/CapsuleComponent.h"
#include "Util/CustomLog.h"

// Sets default values
AElevator::AElevator()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	RootComponent = Platform;

	PressurePlate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PressurePlate"));
	PressurePlate->SetRelativeLocation(FVector(0.f, 0.f, 5.f));
	PressurePlate->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.1f));
	PressurePlate->SetupAttachment(RootComponent);

	PlayerDetector = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PlayerDetector"));
	PlayerDetector->InitCapsuleSize(51.f, 180.f);
	PlayerDetector->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
	PlayerDetector->SetupAttachment(PressurePlate);
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();

	PlayerDetector->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnOverlapBegin);
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTriggered && !bIsReached)
	{
		if (bIsUpward) // If the Elevator is Up
		{
			MoveDown(DeltaTime);
		}
		else // If the Elevator is Down
		{
			MoveUp(DeltaTime);
		}
		StopCheck();
	}
}

void AElevator::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsTriggered = true;
}

void AElevator::MoveUp(float DeltaTime)
{
	float	TickDistance = DeltaTime * Speed;
	FVector NewRocation = FVector(0.f, 0.f, TickDistance);

	Platform->AddRelativeLocation(NewRocation);
	MovedDistance += TickDistance;
}

void AElevator::MoveDown(float DeltaTime)
{
	float	TickDistance = DeltaTime * Speed;
	FVector NewRocation = FVector(0.f, 0.f, -TickDistance);

	Platform->AddRelativeLocation(NewRocation);
	MovedDistance += TickDistance;
}

void AElevator::StopCheck()
{
	if (MoveDistance <= MovedDistance)
	{
		bIsReached = true;
		// (Add Pressure Plate Animation)

		FTimerHandle myTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]() {
			bIsUpward = !bIsUpward;
			MovedDistance = 0.f;
			bIsReached = false;
			bIsTriggered = false;
			LOGC("Init Completed!! : bIU %d | bIR %d | bIT %d", bIsUpward, bIsReached, bIsTriggered)

			// 타이머 초기화
			GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
		}),
			3.0f, false); // Animation Length : 3.0 Sec, 반복 실행을 하고 싶으면 false 대신 true 대입
	}
}
