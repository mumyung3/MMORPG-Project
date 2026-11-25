// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/InteractObject.h"
#include "Savepoint.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API ASavepoint : public AInteractObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = ID, meta = (AllowPrivateAccess = "true"))
	FName SavepointID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SavepointMesh;

	UPROPERTY(EditAnywhere, Category = "Savepoint Marker UI", meta = (AllowPrivateAccess = "true"))
	class USavepointDataAsset* SavepointDataAsset; // 세이브포인터 데이터 에셋
	
public:
	ASavepoint();

	virtual void OnEnterInteract(APawn* Interactor) override;
	virtual void OnInteract() override;
	virtual void OnExitInteract() override;

	virtual FText GetInteractDesc() override;

	// ** Getter Function *
	FName GetSavepointID();

	// ** Print Position by Log (로그용 함수) *
	void LogPosition();
	
protected:
	virtual void BeginPlay() override;
};
