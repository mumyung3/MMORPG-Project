
#include "Base/BaseComp.h"
#include "Character/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"
#include "Kismet/GameplayStatics.h"
#include "Util/CustomLog.h"

UBaseComp::UBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UBaseComp::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 오버라이드 및 슈퍼 꼭 하기
void UBaseComp::InitializeComponent()
{
	Super::InitializeComponent();

	Player = Cast<AWarriorHeroCharacter>(GetOwner());

	// Client Code.
	if (!GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		// 다른 CLIENT은 PC가 없음을 이용
		if (  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PC = Cast<AWarriorHeroController>(PlayerController);
		}
	}
}
