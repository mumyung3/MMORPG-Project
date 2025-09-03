// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/MAPLEGameMode.h"

#include "UObject/ConstructorHelpers.h"


AMAPLEGameMode::AMAPLEGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/LYH/PlayerCharacter/BP_HeroCharacter"));
	
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
