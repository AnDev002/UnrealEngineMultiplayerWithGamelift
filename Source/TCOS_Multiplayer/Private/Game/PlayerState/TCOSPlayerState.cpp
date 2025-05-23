// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PlayerState/TCOSPlayerState.h"
#include "AbilitySystem/TCOSAbilitySystemComponent.h"
#include "UI/HTTP/HttpRequestTypes.h"
#include "AbilitySystem/Attributes/TCOSAttributeSet.h"

ATCOSPlayerState::ATCOSPlayerState()
{
    bReplicates = true;
    bAlwaysRelevant = true;
    NetUpdateFrequency = 100.f;
    MinNetUpdateFrequency = 66.f;

    TCOSAbilitySystemComp = CreateDefaultSubobject<UTCOSAbilitySystemComponent>("AbilitySystemComp");
    TCOSAbilitySystemComp->SetIsReplicated(true);
    TCOSAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    TCOSAttributes = CreateDefaultSubobject<UTCOSAttributeSet>("AttributeSet");


    Score = 0;
    Defeats = 0;
    Hits = 0;
    HighestStreak = 0;
    bFirstBlood = false;
    bWinner = false;
}

void ATCOSPlayerState::OnMatchEnded(const FString& Username)
{
    Super::OnMatchEnded(Username);

    FDSRecordMatchStatsInput RecordMatchStatsInput;
    RecordMatchStatsInput.username = Username;

    RecordMatchStatsInput.matchStats.score = Score;
    RecordMatchStatsInput.matchStats.defeats = Defeats;
    RecordMatchStatsInput.matchStats.hits = Hits;
    RecordMatchStatsInput.matchStats.highestStreak = HighestStreak;
    RecordMatchStatsInput.matchStats.gotFirstBlood = bFirstBlood ? 1 : 0;
    RecordMatchStatsInput.matchStats.matchWins = bWinner ? 1 : 0;
    RecordMatchStatsInput.matchStats.matchLosses = bWinner ? 0 : 1;

    RecordMatchStats(RecordMatchStatsInput);
}

UAbilitySystemComponent* ATCOSPlayerState::GetAbilitySystemComponent() const
{
    return TCOSAbilitySystemComp;
}

UTCOSAbilitySystemComponent* ATCOSPlayerState::GetTCOSAbilitySystemComponent() const
{
    return TCOSAbilitySystemComp;
}

UTCOSAttributeSet* ATCOSPlayerState::GetTCOSAttribute() const
{
    return TCOSAttributes;
}
