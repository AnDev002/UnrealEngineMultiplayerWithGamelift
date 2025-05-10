// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Types/DSTypes.h"
#include "DSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADSGameModeBase : public AGameMode
{
	GENERATED_BODY()
	

protected:
	void StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);
	void StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);
	void UpdateCountdownTimer(const FCountdownTimerHandle& CountdownTimerHandle);
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type);
	void TrySeamlessTravel(TSoftObjectPtr<UWorld> DestinationMap);
	void RemovePlayerSession(AController* Exiting);
};
