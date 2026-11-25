
#include "Base/InputComp.h"

#include "Character/WarriorHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Util/CustomLog.h"

UInputComp::UInputComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInputComp::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInputComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
// CallBack Function
void UInputComp::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
}
