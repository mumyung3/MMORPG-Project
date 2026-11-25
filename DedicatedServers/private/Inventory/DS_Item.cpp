#include "Inventory/DS_Item.h"

#include "Components/InterpToMovementComponent.h"
#include "DSP/DelayStereo.h"
#include "DataTypes/Inventory/InventoryTypes.h"
#include "Net/UnrealNetwork.h"
#include "Util/CustomLog.h"

ADS_Item::ADS_Item()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoneyMesh"));

	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);

	Item.Quantity = 1;

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTableObject(TEXT("/Game/GDH/Inventory/Item/DT_Items"));
	if (ItemTableObject.Succeeded()) ItemDataTable = ItemTableObject.Object;

	MeshComp->SetCollisionProfileName("BlockAllDynamic");

	// 서버 최적화
	if (GetNetMode() == NM_DedicatedServer)
	{
		MeshComp->SetForcedLodModel(1);
	}
	

	
}

void ADS_Item::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	OnRep_InitMesh();
}
void ADS_Item::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADS_Item, Item);
}

void ADS_Item::BeginPlay()
{
	Super::BeginPlay();

	
	if(HasAuthority() && (bIsNotDestroy == false))
	{
		SetReplicateMovement(true); // 이동 복제 활성화

		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ADS_Item::DestroyMyself, 30.0f, false);

	}
	OnRep_InitMesh();
}

void ADS_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADS_Item::DestroyMyself(){

	Destroy();

}

void ADS_Item::OnRep_InitMesh(){
	if (!ItemDataTable || Item.ItemTableRowHandle.RowName.IsNone()) {
		UE_LOG(LogTemp, Warning, TEXT("DataTable이 비어있거나 RowName이 None입니다."));
		return;
	}
    
	FItem* ItemData = ItemDataTable->FindRow<FItem>(Item.ItemTableRowHandle.RowName, TEXT(""));
	if (!ItemData) return;
    
	if (MeshComp) {
		MeshComp->SetStaticMesh(ItemData->Mesh);
	}
}