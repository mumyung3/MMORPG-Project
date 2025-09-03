#pragma once

#include "CoreMinimal.h"
#include "GameEnums.generated.h"

UENUM(BlueprintType)
enum class EInputModeType : uint8
{
	GameOnly     UMETA(DisplayName = "Game Only"),
	UIOnly       UMETA(DisplayName = "UI Only"),
	GameAndUI    UMETA(DisplayName = "Game and UI")
};

UENUM(BlueprintType)
enum class EUIType : uint8
{
	MainMenu,
	Inventory,
	SavePointMenu,
	WorldMap,
	ConfirmationDialog,
	NPCDialog,
	NPCMenu,
	NPCConversation
};

UENUM(BlueprintType)
enum class ENPCRole : uint8
{
	Villager,
	Merchant,
	Blacksmith
};

UENUM(BlueprintType)
enum class ENPCMenu : uint8
{
	Shop,
	Upgrade,
	Talk,
	Exit
};