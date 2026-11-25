// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseComp.h"
#include "Object/ItemData.h"

#include "InventoryComp.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API UInventoryComp : public UBaseComp
{
	GENERATED_BODY()

public:
	UInventoryComp();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TArray<FItemData> Items;

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

public:
	void AddItem(const FItemData& PickedItem);

	bool RemoveItem(const FString& ItemID);

	bool HasItem(const FString& ItemID);
};
