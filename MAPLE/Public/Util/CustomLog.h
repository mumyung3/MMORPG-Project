// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Log Category EXTERN
MAPLE_API DECLARE_LOG_CATEGORY_EXTERN(A_, Log, All)
MAPLE_API DECLARE_LOG_CATEGORY_EXTERN(B_, Log, All)
MAPLE_API DECLARE_LOG_CATEGORY_EXTERN(C_, Log, All)
MAPLE_API DECLARE_LOG_CATEGORY_EXTERN(D_, Log, All)

// Current Executing Function Name
#define CLASS_INFO (FString(__FUNCTION__) + TEXT(" Code(") + FString::FromInt(__LINE__) + TEXT(")"))
// USING ONLY IN GAME 
#define WHERESERVER (GetWorld() ?\
(GetWorld()->IsNetMode(NM_DedicatedServer) ? TEXT("DedicatedServer") : TEXT("Client"))\
: TEXT("Unknown"))
/**
 *
 * @post "STRING" 인자 여러개 사용 하시면 됨. 가변이므로 여러개 인자 가능
 * 카테고리는 남들과 안겹치도록 위처럼 확장해서 CustomLOG처럼 쓰기 
 * @param fmt "STRING"
 */
#define LOGA(fmt, ...) UE_LOG(A_, Warning, TEXT("%s | Role:%s | %s | %s"), *GetName(), *CLASS_INFO, *FString(WHERESERVER), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))
#define LOGB(fmt, ...) UE_LOG(B_, Warning, TEXT("%s | Role:%s | %s | %s"), *GetName(), *CLASS_INFO, *FString(WHERESERVER), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))
#define LOGC(fmt, ...) UE_LOG(C_, Warning, TEXT("%s | Role:%s | %s | %s"), *GetName(), *CLASS_INFO, *FString(WHERESERVER), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))
#define LOGD(fmt, ...) UE_LOG(D_, Warning, TEXT("%s | Role:%s | %s | %s"), *GetName(), *CLASS_INFO, *FString(WHERESERVER), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))


