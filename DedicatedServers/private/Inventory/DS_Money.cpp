// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/DS_Money.h"

#include "Components/InterpToMovementComponent.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "MAPLE/Public/Util/CustomLog.h"

// Sets default values
ADS_Money::ADS_Money()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// 메쉬 컴포넌트 추가 및 초기화
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoneyMesh"));
	
	// RootComponent로 설정 (필요 시 변경 가능)
	RootComponent = MeshComp;

	InterpToMovementComponent = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("InterpToMovementComponent"));

}

void ADS_Money::OnConstruction(const FTransform& Transform)
{
	AActor::OnConstruction(Transform);

	if (!MoneyDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Money의 DataTable이 비어있습니다."));
		return;
	}
	// DataTable 및 RowName 검증
	if (!MoneyTableRowHandle.DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Money의 ItemTableRowHandle의 DataTable이 비어있습니다."));
		return;
	}
	
	FMoney* Money = MoneyDataTable->FindRow<FMoney>(MoneyTableRowHandle.RowName, TEXT(""));
	if (!Money) return;
	MeshComp->SetStaticMesh(Money->Mesh);
}

// Called when the game starts or when spawned
void ADS_Money::BeginPlay()
{
	Super::BeginPlay();

	// InterpToMovementComponent->Duration = 3.0f;
	// InterpToMovementComponent->BehaviourType = EInterpToBehaviourType::PingPong;
	//
	// // BeginPlay()에서
	// FInterpControlPoint Point1, Point2;
	// Point1.PositionControlPoint = FVector(0, 0, 30);     // 시작점
	// Point2.PositionControlPoint = FVector(0, 0, -30);   // 200유닛 위
	// InterpToMovementComponent->ControlPoints.Add(Point1);
	// InterpToMovementComponent->ControlPoints.Add(Point2);

	if(HasAuthority())
	{
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ADS_Money::DestroyMyself, 30.0f, false);
	}
}

// Called every frame
void ADS_Money::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADS_Money::DestroyMyself()
{
	Destroy();
}