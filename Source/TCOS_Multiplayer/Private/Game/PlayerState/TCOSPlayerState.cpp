// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PlayerState/TCOSPlayerState.h"
#include "AbilitySystem/TCOSAbilitySystemComponent.h"
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
