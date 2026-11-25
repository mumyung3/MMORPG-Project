// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "GameFramework/PlayerStart.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAPLE_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	template <class T>
	T* GetOwningPawn() const {
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template Parameter to GetPawn must be derived from APawn"); 
		//T must be APawn, or Compile error
		return CastChecked<T>(GetOwner());
		// CastChecked is safe type casting function, it's same like static_cast or dynamic_cast. 
	}

	APawn* GetOwningPawn() const {
		return GetOwningPawn<APawn>();
	}

	
	template <class T>
	T* GetOwningController() const {
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template Parameter to GetController must be derived from AController");
		return GetOwningPawn<APawn>()->GetController<T>();
	}

	template <class T>
	T* GetOwningState() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APlayerState>::Value, "'T' Template Parameter to GetController must be derived from APlayerState");
		return GetOwningPawn<APawn>()->GetPlayerState<T>();
	}
};
