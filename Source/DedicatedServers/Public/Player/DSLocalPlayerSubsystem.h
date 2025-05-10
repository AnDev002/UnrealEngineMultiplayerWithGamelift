// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UI/HTTP/HttpRequestTypes.h"
#include "DSLocalPlayerSubsystem.generated.h"

class IPortalManagement;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDSLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	void InitializeTokens(const FAuthenticationResult& AuthResult, TScriptInterface<IPortalManagement> PortalManagement);
	void SetRefreshTokenTimer();
	void UpdateTokens(const FString& AccessToken, const FString& IdToken);
	FAuthenticationResult GetAuthResult() const;

	FString Username;
	FString Email;
private:
	UPROPERTY()
	FAuthenticationResult AuthenticationResult;

	UPROPERTY()
	TScriptInterface<IPortalManagement> PortalManagementInterface;
	
	float TokenRefreshInterval = 2700.f; // 75% of an hour (the expiration time for AccessToken and IdD)
	FTimerHandle RefreshTimer;
};
