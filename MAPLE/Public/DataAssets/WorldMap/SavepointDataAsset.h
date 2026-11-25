// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SavepointDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSavepointData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SavepointID{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SavepointName{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector WorldPosition = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MapPosition = FVector2D(0.0f, 0.0f);
};

UCLASS()
class MAPLE_API USavepointDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSavepointData> SavepointList;

	// SavepointList 에서 인자로 받아온 ID값이 들어있는 세이브포인트 데이터 리턴
	FSavepointData* FindDataByID(FName ID)
	{
		return SavepointList.FindByPredicate([&](FSavepointData& Data)
		{
			return Data.SavepointID == ID;
		});
	}
};
