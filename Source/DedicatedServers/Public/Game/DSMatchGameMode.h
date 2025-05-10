// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DSGameModeBase.h"
#include "DSMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADSMatchGameMode : public ADSGameModeBase
{
	GENERATED_BODY()
	
public:
	ADSMatchGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	UPROPERTY()
	EMatchStatus MatchStatus;

protected:
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimer;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimer;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;

	void SetClientInputEnabled(bool bEnabled);
};
