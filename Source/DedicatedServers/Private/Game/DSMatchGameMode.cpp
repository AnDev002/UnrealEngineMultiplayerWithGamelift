// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DSMatchGameMode.h"
#include "Player/DSPlayerController.h"
#include "Player/DSMatchPlayerState.h"

ADSMatchGameMode::ADSMatchGameMode()
{
	bUseSeamlessTravel = true;
	MatchStatus = EMatchStatus::WaitingForPlayers;
	PreMatchTimer.Type = ECountdownTimerType::PreMatch;
	MatchTimer.Type = ECountdownTimerType::Match;
	PostMatchTimer.Type = ECountdownTimerType::PostMatch;
}

void ADSMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		StartCountdownTimer(PreMatchTimer);
	}
}

void ADSMatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerSession(Exiting);
}

void ADSMatchGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	if (MatchStatus == EMatchStatus::WaitingForPlayers)
	{
		MatchStatus = EMatchStatus::PreMatch;
		StartCountdownTimer(PreMatchTimer);
	}
}

void ADSMatchGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type);

	if (Type == ECountdownTimerType::PreMatch)
	{
		StopCountdownTimer(PreMatchTimer);
		MatchStatus = EMatchStatus::Match;
		StartCountdownTimer(MatchTimer);
		SetClientInputEnabled(true);
	}
	if (Type == ECountdownTimerType::Match)
	{
		StopCountdownTimer(PreMatchTimer);
		MatchStatus = EMatchStatus::PostMatch;
		StartCountdownTimer(PostMatchTimer);
		SetClientInputEnabled(true);
		OnMatchEnded();
	}
	if (Type == ECountdownTimerType::PostMatch)
	{
		StopCountdownTimer(PreMatchTimer);
		MatchStatus = EMatchStatus::SeamlessTravelling;
		TrySeamlessTravel(LobbyMap);
	}
}

void ADSMatchGameMode::SetClientInputEnabled(bool bEnabled)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get());
		if (IsValid(DSPlayerController))
		{
			DSPlayerController->Client_SetInputEnabled(bEnabled);
		}
	}
}

void ADSMatchGameMode::OnMatchEnded()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Iterator->Get()); IsValid(DSPlayerController))
		{
			if (ADSMatchPlayerState* MatchPlayerState = DSPlayerController->GetPlayerState<ADSMatchPlayerState>(); IsValid(MatchPlayerState))
			{
				MatchPlayerState->OnMatchEnded(DSPlayerController->Username);
			}
		}
	}
}




