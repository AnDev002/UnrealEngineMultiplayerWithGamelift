// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DSMatchPlayerState.generated.h"

struct FDSRecordMatchStatsInput;
class UGameStatsManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADSMatchPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void OnMatchEnded(const FString& Username);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;
protected:
	virtual void BeginPlay() override;
	void RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput) const;
private:

	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;
	
	
	
};
