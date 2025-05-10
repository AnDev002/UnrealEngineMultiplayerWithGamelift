// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSGameModeBase.h"
#include "GameLiftServerSDK.h"
#include "DSLobbyGameMode.generated.h"

class UDSGameInstanceSubsystem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADSLobbyGameMode : public ADSGameModeBase
{
	GENERATED_BODY()
	
public:
	ADSLobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
protected:
	virtual void BeginPlay() override;
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	void CancelCountdown();
	virtual void Logout(AController* Exiting) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;
	UPROPERTY()
	ELobbyStatus LobbyStatus;

	UPROPERTY(EditDefaultsOnly)
	int32 MinPlayers;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MapToTravelTo;

private:

	FProcessParameters ProcessParameters;

	UPROPERTY()
	TObjectPtr<UDSGameInstanceSubsystem> DSGameInstanceSubsystem;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle LobbyCountdownTimer;

	void InitGameLift();

	void SetServerParameters(FServerParameters& OutServerParameters);
	void TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage);
	/*void AddPlayerInfoToLobbyState(AController* Player) const;
	void RemovePlayerInfoFromLobbyState(AController* Player) const;*/

	
	
};
