// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TCOSGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayAbilitySpec.h"
#include "HitReactionAbility.generated.h"

/**
 * 
 */
enum class EHitDirection : uint8;

UCLASS()
class TCOS_MULTIPLAYER_API UHitReactionAbility : public UTCOSGameplayAbility
{
	GENERATED_BODY()

    UHitReactionAbility();
public:

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    EHitDirection CalculateHitDirection(const AActor* Victim, const AActor* Attacker);

    //virtual bool ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* Payload) const override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Tags")
    FGameplayTagContainer GameplayEventTags;


};
