#pragma once
#include "DataTypes/Enums/Enums.h"

#include "InventoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FMoney : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Name{};

	UPROPERTY(EditAnywhere)
	int32 Amount{};

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh{};

	void Dump() const;
};


USTRUCT(BlueprintType)
struct DEDICATEDSERVERS_API FItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Name{};
	
	UPROPERTY(EditAnywhere)
	FString Description{};

	UPROPERTY(EditAnywhere)
	UTexture2D* Thumbnail{};

	UPROPERTY(EditAnywhere)
	int32 StackSize{};

	UPROPERTY(EditAnywhere)
	int32 Power{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh{};
	
	//void Dump() const;
};

USTRUCT(BlueprintType)
struct FSlot 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ItemTableRowHandle{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType{EItemType::None};
	
	//void Dump() const;
};


USTRUCT(BlueprintType)
struct FALLItem 
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	TArray<FSlot> Swords{};

	UPROPERTY(EditAnywhere)
	TArray<FSlot> Shields{};


	//인벤토리 배열
	UPROPERTY(EditAnywhere)
	TArray<FSlot> Eatables{};
};