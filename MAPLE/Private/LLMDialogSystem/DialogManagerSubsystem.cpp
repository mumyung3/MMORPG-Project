
#include "LLMDialogSystem/DialogManagerSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/ConfigCacheIni.h"

void UDialogManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDialogManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDialogManagerSubsystem::StartConversation(const FName& SpeakerName, const FString& Dialog)
{
	if (!IsRunningOnClient())
	{
		return;
	}

	// UE_LOG(LogTemp, Display, TEXT("StartConversation called by %s with dialog: %s"), *SpeakerName.ToString(), *Dialog);

	FString FormattedDialog = SpeakerName.ToString() + ": " + Dialog;
	CallGeminiAPI(FormattedDialog);
}

void UDialogManagerSubsystem::AddToDialogHistory(const FName& SpeakerName, const FString& Dialog)
{
	FDialogData DialogData;
	DialogData.DialogID = DialogHistory.Num();
	DialogData.SpeakerName = SpeakerName;
	DialogData.Dialog = Dialog;
	DialogHistory.Add(DialogData);
	OnDialogHistoryUpdated.Broadcast();
}

void UDialogManagerSubsystem::ClearDialogHistory()
{
	DialogHistory.Empty();
	OnDialogHistoryUpdated.Broadcast();
}

void UDialogManagerSubsystem::CallGeminiAPI(const FString& Prompt)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	FString APIKey;
	if (GConfig)
	{
		GConfig->GetString(
			TEXT("Cred"),
			TEXT("GEMINI_API_KEY"),
			APIKey,
			FPaths::ProjectConfigDir() / TEXT("Cred.ini"));
	}

	Request->SetURL("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash-preview-04-17:generateContent?key=" + APIKey);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	TSharedPtr<FJsonObject> RequestData = MakeShareable(new FJsonObject);

	FString SystemPrompt = "You are a helpful NPC in a game. Use a natural, conversational tone appropriate for a game character. Respond in Korean. History:\n";

	for (FDialogData DialogData : DialogHistory)
	{
		SystemPrompt = SystemPrompt + DialogData.SpeakerName.ToString() + ": " + DialogData.Dialog + '\n';
	}

	TArray<TSharedPtr<FJsonValue>> ContentsArray;

	// First message - System prompt
	{
		TSharedPtr<FJsonObject> ContentObject = MakeShareable(new FJsonObject);
		ContentObject->SetStringField("role", "user");

		TArray<TSharedPtr<FJsonValue>> PartsArray;
		TSharedPtr<FJsonObject>		   TextPart = MakeShareable(new FJsonObject);
		TextPart->SetStringField("text", SystemPrompt);
		PartsArray.Add(MakeShareable(new FJsonValueObject(TextPart)));

		ContentObject->SetArrayField("parts", PartsArray);
		ContentsArray.Add(MakeShareable(new FJsonValueObject(ContentObject)));
	}

	// Second message - User prompt
	{
		TSharedPtr<FJsonObject> ContentObject = MakeShareable(new FJsonObject);
		ContentObject->SetStringField("role", "user");

		TArray<TSharedPtr<FJsonValue>> PartsArray;
		TSharedPtr<FJsonObject>		   TextPart = MakeShareable(new FJsonObject);
		TextPart->SetStringField("text", Prompt);
		PartsArray.Add(MakeShareable(new FJsonValueObject(TextPart)));

		ContentObject->SetArrayField("parts", PartsArray);
		ContentsArray.Add(MakeShareable(new FJsonValueObject(ContentObject)));
	}

	RequestData->SetArrayField("contents", ContentsArray);

	AddToDialogHistory(FName("Player"), Prompt);

	FString					  RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestData.ToSharedRef(), Writer);

	Request->SetContentAsString(RequestBody);
	Request->OnProcessRequestComplete().BindUObject(this, &UDialogManagerSubsystem::OnResponseReceived);

	// UE_LOG(LogTemp, Display, TEXT("API Request Body: %s"), *RequestBody);
	Request->ProcessRequest();
}
void UDialogManagerSubsystem::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to call Gemini API"));
		return;
	}

	FString ResponseString = Response->GetContentAsString();
	UE_LOG(LogTemp, Log, TEXT("Response: %s"), *ResponseString);

	TSharedPtr<FJsonObject>	  JsonResponse;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

	if (!FJsonSerializer::Deserialize(Reader, JsonResponse) || !JsonResponse.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize Gemini API response"));
		return;
	}

	const TArray<TSharedPtr<FJsonValue>>* CandidatesArray;
	if (!JsonResponse->TryGetArrayField("candidates", CandidatesArray) || CandidatesArray->Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No candidates available in the response"));
		return;
	}

	TSharedPtr<FJsonObject> CandidateObject = (*CandidatesArray)[0]->AsObject();
	if (!CandidateObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid candidate object"));
		return;
	}

	const TSharedPtr<FJsonObject>* ContentObjectPtr = nullptr;
	if (!CandidateObject->TryGetObjectField("content", ContentObjectPtr) || !ContentObjectPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("No content object available in the candidate"));
		return;
	}

	TSharedPtr<FJsonObject> ContentObject = *ContentObjectPtr;
	if (!ContentObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid content object"));
		return;
	}

	const TArray<TSharedPtr<FJsonValue>>* PartsArray;
	if (!ContentObject->TryGetArrayField("parts", PartsArray) || PartsArray->Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No parts available in the content"));
		return;
	}

	TSharedPtr<FJsonObject> PartObject = (*PartsArray)[0]->AsObject();
	if (!PartObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid part object"));
		return;
	}

	FString TextContent;
	if (!PartObject->TryGetStringField("text", TextContent))
	{
		UE_LOG(LogTemp, Error, TEXT("No text available in the part"));
		return;
	}

	ParseMessageContent(TextContent);
}

void UDialogManagerSubsystem::ParseMessageContent(const FString& Content)
{
	// UE_LOG(LogTemp, Log, TEXT("Content: %s"), *Content);

	AddToDialogHistory(NPCName, Content);
}

bool UDialogManagerSubsystem::IsRunningOnClient() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	return World->GetNetMode() != NM_DedicatedServer;
}
