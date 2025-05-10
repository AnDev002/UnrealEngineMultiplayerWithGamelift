// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TCOSGameplayAbility.h"
#include "AbilitySystem/Abilities/TCOSDamageAbility.h"
#include "AbilitySystem/TCOSAbilityTypes.h"
#include "PickupItemAbility.generated.h"

/**
 * 
 */
UCLASS()
class TCOS_MULTIPLAYER_API UPickupItemAbility : public UTCOSDamageAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
