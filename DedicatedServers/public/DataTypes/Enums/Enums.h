#pragma once

//로비
UENUM(BlueprintType)
enum class ELobbyMenuType : uint8
{
	LobbyMenu  UMETA(DisplayName = "LOBBYMENU"),
	ServerList UMETA(DisplayName = "SERVERLIST"),
	Configure  UMETA(DisplayName = "CONFIGURE"),
	Exit	   UMETA(DisplayName = "EXIT"),
	ListFleets UMETA(DisplayName = "LISTFLEETS"),
	None	   UMETA(DisplayName = "NONE")
};

// 아이템 종류 (소드, 방패, 소비)
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "NONE"),
	Sword UMETA(DisplayName = "SWORD"),
	Shield UMETA(DisplayName = "SHIELD"),
	Eatables UMETA(DisplayName = "EATALBES")
};
