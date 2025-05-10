// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStats/GameStatsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HttpRequestTypes.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
void UGameStatsManager::RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput)
{
	
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FDSRecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonString);


	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString ApiUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameStatsAPI::RecordMatchStats);
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RecordMatchStats_Response);
	Request->SetURL(ApiUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();
}

void UGameStatsManager::RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("RecordMatchStats failed."))
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		ContainsError(JsonObject);
	}
}