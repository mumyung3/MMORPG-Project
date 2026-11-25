// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Combat/RespawnComponent.h"

#include "ARSessionConfig.h"
#include "TimerManager.h"
#include "WarriorFunctionLibrary.h"
#include "Camera/CameraComponent.h"

#include "Character/WarriorHeroCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "Inventory/Manger/UseItemComponent.h"
#include "Util/CustomLog.h"

// Sets default values for this component's properties
URespawnComponent::URespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void URespawnComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);

	if (APawn* Owner = GetOwner<AController>()->GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn State - IsValid: %s, HasController: %s"), 
			   IsValid(Owner) ? TEXT("true") : TEXT("false"),
			   Owner && Owner->GetController() ? TEXT("true") : TEXT("false"));
	}	
}

// Called every frame
void URespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner<AController>()->GetPawn()) return;
	float Speed = GetOwner<AController>()->GetPawn()->GetVelocity().Size();
	float TargetFOV = FMath::Lerp(90.0f, 100.0f, Speed / MaxSpeed);
	if (!GetOwner<AController>()->GetPawn()) return;
	if (!Cast<AWarriorHeroCharacter>(GetOwner<AController>()->GetPawn())) return;
	Cast<AWarriorHeroCharacter>(GetOwner<AController>()->GetPawn())->FollowCamera->SetFieldOfView(FMath::FInterpTo(Cast<AWarriorHeroCharacter>(GetOwner<AController>()->GetPawn())->FollowCamera->FieldOfView, TargetFOV, DeltaTime, 2.0f));
}
void URespawnComponent::OnDeath()
{
	ServerRPCChangeToSpectator();
	ClientRPCStopMusic();

}
void URespawnComponent::ServerRPCRespawnPlayer()
{

	APawn* Player = GetOwner<AController>()->GetPawn();
	//  2. 얘를 Unpossess
	GetOwner<AController>()->UnPossess();

	// 3.5 태그 삭제 플레이어 스테이트에 있는지 캐릭터에 있는지
	if (AWarriorBaseCharacter* WarriorHeroCharacter = Cast<AWarriorBaseCharacter>(GetOwner<AController>()->GetPawn()))
	{
		UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(WarriorHeroCharacter, FGameplayTag::RequestGameplayTag("Shared.Status.Dead"));
	}

	//  3. Destroy
	Player->Destroy();
	// 4. respawn
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

	GameMode->RestartPlayer(GetOwner<AController>());

	// 3.5 태그 삭제 플레이어 스테이트에 있는지 캐릭터에 있는지
	if (AWarriorBaseCharacter* WarriorHeroCharacter = Cast<AWarriorBaseCharacter>(GetOwner<AController>()->GetPawn()))
	{
		UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(WarriorHeroCharacter, FGameplayTag::RequestGameplayTag("Shared.Status.Dead"));
	}

	bIsRespawn = false;
}

void URespawnComponent::ServerRPCChangeToSpectator_Implementation()
{
	ChangeToSpectator();
}

void URespawnComponent::ClientRPCStopMusic_Implementation(){

	
	if (!GetOwner<AController>()->GetPawn()) return;
	if (!Cast<AWarriorHeroCharacter>(GetOwner<AController>()->GetPawn())) return;

	Cast<AWarriorHeroCharacter>(GetOwner<AController>()->GetPawn())->PickUpComp->StopSound();
}

void URespawnComponent::ChangeToSpectator()
{

	if (!GetOwner<AController>()->GetPawn())
		return;
	if (!Cast<AWarriorHeroCharacter>(GetOwner<AController>()->GetPawn()))
		return;

	// //if (auto* HeroCharacter = Cast<AWarriorHeroCharacter>(GetOwner<AController>()->GetPawn()))
	// {
	// 	// FGameplayTag HealInputTag = FGameplayTag::RequestGameplayTag(FName("InputTag.UnequipSword"));
	// 	// HeroCharacter->Input_AbilityInputPressed(HealInputTag);
	// }

	if (bIsRespawn)
		return;
	bIsRespawn = true;

	// 1. 기존 사용하던 pawn 가져오기
	APawn*				   Player = GetOwner<AController>()->GetPawn();
	AWarriorHeroCharacter* MyCharacter = Cast<AWarriorHeroCharacter>(GetOwner<AController>()->GetPawn());

	// 2. spawn 관전자 pawn
	//  -> Transform
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

	auto Spectator = GameMode->SpectatorClass;
	auto Trans = Player->GetActorTransform();
	auto SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(Spectator, Trans, Params);
	// 3. Possess
	GetOwner<AController>()->Possess(SpectatorPawn);
	// 4. 기존 pawn 제거
	Player->Destroy();

	// 5초후에 리스폰 처리
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &URespawnComponent::ServerRPCRespawnPlayer, 5, false);
}


// 서버로 실행해줘야함.
void URespawnComponent::ForceRespawn()
{
	
	// 클라 노래 꺼주기
	ClientRPCStopMusic();
	ServerRPCForceRespawn();
	
}

void URespawnComponent::ServerRPCForceRespawn_Implementation(){

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &URespawnComponent::ChangeToSpectator, 3.5, false);
	
}