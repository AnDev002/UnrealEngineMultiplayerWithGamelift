// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "Interfaces/IHttpRequest.h"
#include "GameSessionsManager.generated.h"
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UGameSessionsManager : public UHTTPRequestManager
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessage BroadcastJoinGameSessionMessage;

	void JoinGameSession();

private:

	void FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FString	GetUniquePlayerId() const;

	void HandleGameSessionStatus(const FString& Status, const FString& SessionId);

	void TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionId);

	FTimerHandle CreateSessionTimer;

	FTimerHandle RetryCreateSessionTimerHandle;
	
};
