// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "DialogManagerSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FDialogData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 DialogID{};

	UPROPERTY(BlueprintReadWrite)
	FName SpeakerName{};

	UPROPERTY(BlueprintReadWrite)
	FString Dialog{};
};
/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogHistoryUpdated);

UCLASS()
class MAPLE_API UDialogManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FName NPCName;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Dialog", meta = (DisplayName = "Start Conversation"))
	void StartConversation(const FName& SpeakerName, const FString& Dialog);

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void AddToDialogHistory(const FName& SpeakerName, const FString& Dialog);

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void ClearDialogHistory();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	TArray<FDialogData> GetDialogHistory() const { return DialogHistory; }

	UPROPERTY(BlueprintAssignable, Category = "Dialog")
	FOnDialogHistoryUpdated OnDialogHistoryUpdated;

	void SetNPCName(FName InNPCName) { NPCName = InNPCName; }

private:
	TArray<FDialogData> DialogHistory;

	void CallGeminiAPI(const FString& Prompt);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void ParseMessageContent(const FString& Content);

	bool IsRunningOnClient() const;
};
