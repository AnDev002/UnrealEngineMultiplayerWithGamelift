// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSMatchPlayerState.h"

#include "UI/GameStats/GameStatsManager.h"

void ADSMatchPlayerState::OnMatchEnded(const FString& Username)
{
}

void ADSMatchPlayerState::BeginPlay()
{
	Super::BeginPlay();
	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
}

void ADSMatchPlayerState::RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput) const
{
	check(IsValid(GameStatsManager));
	GameStatsManager->RecordMatchStats(RecordMatchStatsInput);
}
