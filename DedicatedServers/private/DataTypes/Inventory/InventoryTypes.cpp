#include "DataTypes/Inventory/InventoryTypes.h"

void FMoney::Dump() const
{
	UE_LOG(LogTemp, Log, TEXT("FMoney Dump:"));
	UE_LOG(LogTemp, Log, TEXT(" - Name: %s"), *Name);
	UE_LOG(LogTemp, Log, TEXT(" - Amount: %d"), Amount);
    
	if (Mesh)
	{
		UE_LOG(LogTemp, Log, TEXT(" - Mesh: %s"), *Mesh->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT(" - Mesh: nullptr"));
	}
}

