// Fill out your copyright notice in the Description page of Project Settings.

#include "HTTP/ListFleets/ListFleetsManager.h"

#include "DedicatedServers.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "DataTypes/DataAsset/APIData.h"
#include "DataTypes/HTTPRequestTypes/HTTPRequestTypes.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
void UListFleetsManager::ListFleets()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ListFleetsButtonClicked"));

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	
	Request->OnProcessRequestComplete().BindUObject(this, &UListFleetsManager::ListFleets_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::ListFleets);
	Request->SetURL(APIUrl);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("List Fleets Request Made"));

	
}	

void UListFleetsManager::ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("List Fleets Response Received"));

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {

    	if (ContainsErrors(JsonObject))
    	{
    		OnListFleetsResponseReceived.Broadcast(FDSListFleetsResponse(), false);
    		return;
    	}
    	DumpMetaData(JsonObject);
    	
        if (JsonObject->HasField(TEXT("$metadata")))
        {
        	TSharedPtr<FJsonObject> MetadataJsonObject = JsonObject->GetObjectField(TEXT("$metadata"));
        	FDSMetaData DSMetaData;
        	FJsonObjectConverter::JsonObjectToUStruct(MetadataJsonObject.ToSharedRef(), &DSMetaData);

        	DSMetaData.Dump();
        	
        }

    	FDSListFleetsResponse ListFleetsResponse;
    	//하위 필드 안거치고 내부적으로 데이터 비교하며 필드 찾아감.
    	FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &ListFleetsResponse);
    	ListFleetsResponse.Dump();

    	OnListFleetsResponseReceived.Broadcast(ListFleetsResponse, true);

    }
	
}