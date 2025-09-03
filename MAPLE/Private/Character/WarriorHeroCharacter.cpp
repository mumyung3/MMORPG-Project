// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WarriorHeroCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/WarriorInputComponent.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "WarriorGameplayTags.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Player/IngamePlayerState.h"

#include "WarriorDebugHelper.h"
#include "Components/Combat/RespawnComponent.h"
#include "Controllers/WarriorHeroController.h"
#include "Input/InteractInputComp.h"
#include "Input/LockPickInputComp.h"
#include "Inventory/Manger/PickUpCompnent.h"
#include "Inventory/Manger/UseItemComponent.h"
#include "Player/InventoryComp.h"
#include "UI/UIComp.h"
#include "Util/CustomLog.h"

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// ** Components *
	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
	HeroCombatComponent->SetIsReplicated(true);

	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));

	UIComponent = CreateDefaultSubobject<UUIComp>(TEXT("UIComponent"));
	InteractInputComp = CreateDefaultSubobject<UInteractInputComp>(TEXT("InteractInputComp"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComp>(TEXT("InventoryComponent"));
	LockPickInputComp = CreateDefaultSubobject<ULockPickInputComp>(TEXT("LockPickInputComp"));
	PickUpComp = CreateDefaultSubobject<UUseItemComponent>(TEXT("PickUpComp"));

	//For pickup CapsuleComp
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(RootComponent);
	CapsuleComp->SetCapsuleSize(65.558334f, 147.311279f); // 반지름, 높이/2
	CapsuleComp->SetCollisionProfileName("OverlapAllDynamic");
	CapsuleComp->SetRelativeLocation(FVector(149.088465f,0.000000f,-29.581109f));
	CapsuleComp->SetRelativeRotation(FRotator(-90.0f, 540.0f, 179.9f));
	
	ShieldMeshComp =CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMeshComp"));
	ShieldMeshComp->SetupAttachment(GetMesh(),FName(TEXT("hand_l_weapon")));
	ShieldMeshComp->SetRelativeLocation(FVector(10.446776,0.167441,-1.635155));
	ShieldMeshComp->SetRelativeRotation(FRotator(0.0f, 12.432823, 71.664105));
}

UPawnCombatComponent* AWarriorHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AWarriorHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

UHeroUIComponent* AWarriorHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}

UUIComp* AWarriorHeroCharacter::GetUIComp()
{
	return UIComponent;
}

UInteractInputComp* AWarriorHeroCharacter::GetInteractInputComp()
{
	return InteractInputComp;
}

UInventoryComp* AWarriorHeroCharacter::GetInventoryComp()
{
	return InventoryComponent;
}

ULockPickInputComp* AWarriorHeroCharacter::GetLockPickInputComp()
{
	return LockPickInputComp;
}

void AWarriorHeroCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	AIngamePlayerState* PS = GetPlayerState<AIngamePlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		WarriorAbilitySystemComponent = Cast<UWarriorAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		WarriorAttributeSet = PS->GetAttributeSetBase();

		WarriorAttributeSet->SetPawnCombatComponent(HeroCombatComponent);
		
		if (!CharacterStartUpData.IsNull())
		{
			if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
			{ // Load DataAsset
				LoadedData->GiveToAbilitySystemComponent(GetWarriorAbilitySystemComponent());
			}
		}
	}
}

void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"))
		ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	// ** GAS Input Binding *
	UWarriorInputComponent* WarriorInputComponent = CastChecked<UWarriorInputComponent>(PlayerInputComponent);

	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	WarriorInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
	// 추가 부분
	UIComponent->SetupPlayerInputComponent(Cast<UEnhancedInputComponent>(PlayerInputComponent));
	PickUpComp->SetupPlayerInputComponent(Cast<UEnhancedInputComponent>(PlayerInputComponent));
}

void AWarriorHeroCharacter::BeginPlay() {
	Super::BeginPlay();
	
}

void AWarriorHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AIngamePlayerState* PS = GetPlayerState<AIngamePlayerState>();
	if (PS) {
		// Set the ASC for clients. Server does this in PossessedBy
		WarriorAbilitySystemComponent = Cast<UWarriorAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor
		WarriorAbilitySystemComponent->InitAbilityActorInfo(PS, this);

		WarriorAttributeSet = PS->GetAttributeSetBase();
	}
}

void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue) {
	if (AWarriorHeroController* PC = Cast<AWarriorHeroController>(Controller))
	{
		if (PC->IsMenuUIOpened())
			return; // 게임 입력 차단
	}
	
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f) {
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f) {
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue) {
	if (AWarriorHeroController* PC = Cast<AWarriorHeroController>(Controller))
	{
		if (PC->IsMenuUIOpened())
			return; // 게임 입력 차단
	}
	
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f) {
		AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f) {
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWarriorHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AWarriorHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X >0.f ? WarriorGameplayTags::Player_Event_SwitchTarget_Right : WarriorGameplayTags::Player_Event_SwitchTarget_Left,
		Data
	);
}

void AWarriorHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag) {
	WarriorAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AWarriorHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	WarriorAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
//서버에서 실행 블루프린트에서 실행됨.
void AWarriorHeroCharacter::ReStartRespawn(){
	
	AWarriorHeroController* PC = GetController<AWarriorHeroController>();

	PickUpComp->StopSound();
	if (!PC) return;
	PC->RespawnComponent->ForceRespawn();
	
}

