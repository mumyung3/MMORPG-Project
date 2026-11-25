// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/Savepoint.h"
#include "Controllers/WarriorHeroController.h"
#include "DataAssets/WorldMap/SavepointDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Player/IngamePlayerState.h"
#include "Util/CustomLog.h"

ASavepoint::ASavepoint()
{
	// SavepointMesh
	SavepointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SavepointMesh"));
	SavepointMesh->SetupAttachment(RootComponent);
}

void ASavepoint::OnEnterInteract(APawn* Interactor)
{
	if (!Interactor)
		return;

	AWarriorHeroController* PC = Cast<AWarriorHeroController>(Interactor->GetController());
	if (!PC) return;

	PC->ShowSavepointMenu();

	AIngamePlayerState* PS = Interactor->GetPlayerState<AIngamePlayerState>(); // PlayerState 받아옴
	if (!PS) return;

	PS->AddDiscoveredSavepoint(SavepointID); // 발견된 세이브포인트 배열에 추가
}

void ASavepoint::OnInteract()
{
}

void ASavepoint::OnExitInteract()
{
}

FText ASavepoint::GetInteractDesc()
{
	return InteractDesc;
}

FName ASavepoint::GetSavepointID()
{
	return SavepointID;
}

void ASavepoint::LogPosition()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	FVector2D ScreenPosition; // 현재 플레이어의 뷰포트에서 보이는 화면 상에서의 액터의 위치 (지금은 필요 X)
	if (UGameplayStatics::ProjectWorldToScreen(PC, GetActorLocation(), ScreenPosition))
	{
		// 월드맵 이미지 크기
		FVector2D CanvasSize = FVector2D(1024.f, 1024.f);

		// 실제 월드맵의 크기
		FVector2D WorldMapOrigin = FVector2D(5260.f, -24880.f); // 월드맵 시작점
		FVector2D WorldMapSize   = FVector2D(38510.f, 21460.f); // 월드맵 크기

		// 세이브포인트의 월드 상 좌표
		FVector2D SavepointLoc2D = FVector2D(GetActorLocation().X, GetActorLocation().Y);

		// 위젯 공간에서의 상대 위치값 (0~1 범위)
		FVector2D NormalizedPos = (SavepointLoc2D - WorldMapOrigin) / WorldMapSize;

		// 월드맵 위젯 중앙 기준 상대 좌표 (-0.5 ~ +0.5)
		FVector2D CenteredPos = NormalizedPos - FVector2D(0.5f, 0.5f);

		// X축 뒤집기
		CenteredPos.X = -CenteredPos.X;

		// 실제 캔버스 내 좌표
		FVector2D MarkerPos = CenteredPos * CanvasSize;

		// X랑 Y랑 위치를 바꿔서 출력하는 이유는 레벨상에서의 좌표계와 위젯상에서의 좌표계가 다르기 때문.
		LOGC("SavepointID: %s | WorldPos: %.1f, %.1f | MapPos: %.4f, %.4f",
			*SavepointID.ToString(),
			SavepointLoc2D.X,
			SavepointLoc2D.Y,
			MarkerPos.Y,
			MarkerPos.X
			);

		// 데이터 에셋에서 일치하는 ID를 찾아 정보 저장.
		FSavepointData* SavepointData = SavepointDataAsset->FindDataByID(SavepointID);
		SavepointData->WorldPosition = GetActorLocation();
		SavepointData->MapPosition.X = MarkerPos.Y;
		SavepointData->MapPosition.Y = MarkerPos.X;
	}
}

void ASavepoint::BeginPlay()
{
	Super::BeginPlay();

	InteractDesc = FText::FromString(TEXT("휴식"));

	// LogPosition();
}
