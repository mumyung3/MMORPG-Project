// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpCompnent.h"
#include "Components/ActorComponent.h"
#include "DragAndDropComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEDICATEDSERVERS_API UDragAndDropComponent : public UPickUpCompnent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDragAndDropComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//드래그 시작한 아이템 슬롯 -> 구조체 배열에서 빼주기.
	void DragStart(int32 Index, EItemType Type);

	void DragEnd(FSlot SourceItem, int32 SourceIndex, int32 DestinationIndex);

	void Drop(const FSlot& Item);
	
	UFUNCTION(Server, Reliable)
	void Server_OnDrop(const FSlot& SlotItem);

	
	void DropAllItem();

	
};
