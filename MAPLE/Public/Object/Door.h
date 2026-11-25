// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/InteractObject.h"
#include "Components/TimelineComponent.h"

#include "Door.generated.h"

/**
 *
 */
UCLASS()
class MAPLE_API ADoor : public AInteractObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DoorFrameMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CollisionBox, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* PlayerDetectBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTimelineComponent> DoorOpenTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> DoorOpenTimelineCurve;

	/** Player is Overlapped with PlayerDetectBox */
	UFUNCTION()
	void BeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor*				 OtherActor,
		UPrimitiveComponent* OtherComp,
		int32				 OtherBodyIndex,
		bool				 bFromSweep,
		const FHitResult&	 SweepResult);

	/** Player is Escaped from PlayerDetectBox */
	UFUNCTION()
	void EndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor*				 OtherActor,
		UPrimitiveComponent* OtherComp,
		int32				 OtherBodyIndex);

	/** Update the Door Movement with the Timeline */
	UFUNCTION(BlueprintCallable)
	virtual void UpdateDoorTimeline(float Output);

	FRotator DoorLerp;

	/** Open or Close the Door */
	UFUNCTION()
	virtual void DoorOpen();

	/** Move the Door | Open -> Close or Close -> Open */
	virtual void MoveDoor(bool bDoorState);

	virtual void OnEnterInteract(APawn* Interactor) override;
	virtual void OnInteract() override;
	virtual void OnExitInteract() override;

	virtual FText GetInteractDesc() override;

public:
	ADoor();

	virtual bool CheckKey(); // Player has RequiredKey : T | else : F

	bool IsDoorLocked(); // Door is Locked : T | else : F

	void SetDoorUnlocked(); // Unlock the Door

protected:
	virtual void BeginPlay() override;

	// ID of the Key Required to Open the Door
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Key, meta = (AllowPrivateAccess = "true"))
	FString RequiredKey;

	// ID of the Door
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = "true"))
	FString DoorID;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* DoorOpenSound;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* DoorUnlockSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundAttenuation* AttenuationSettings;

public:
	// Deligate for Door Opening Timeline Update
	FOnTimelineFloat DoorOpenTimelineUpdateDeligate;

	// Set which variables to replicate.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// RepNotify Function
	UFUNCTION()
	void OnRep_DoorOpen();

	// RepNotify Function
	UFUNCTION()
	void OnRep_SetDoorUnlocked();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayOpenSound();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayUnlockSound();

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_DoorOpen)
	bool bIsOpen; // Door is Open : T | Door is Closed : F

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SetDoorUnlocked)
	bool bIsLocked; // Door is Locked : T | Door is Unlocked : F
};
