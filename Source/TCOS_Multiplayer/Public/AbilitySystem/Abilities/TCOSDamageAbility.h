// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TCOSGameplayAbility.h"
#include "AbilitySystem/TCOSAbilityTypes.h"
#include "TCOSDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class TCOS_MULTIPLAYER_API UTCOSDamageAbility : public UTCOSGameplayAbility
{
	GENERATED_BODY()
	
	
public:
	void CaptureDamageEffectInfo(AActor* TargetActor, FDamageEffectInfo& OutInfo);

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectInfo DamageEffectInfo;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Damage Effect")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Damage Effect")
	FScalableFloat BaseDamage;
	
};
