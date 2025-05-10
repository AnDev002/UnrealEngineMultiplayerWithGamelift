// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TCOSGameplayAbility.h"
#include "DodgeRollAbility.generated.h"

class UAnimMontage;
class ACharacterBase;
/**
 * 
 */
UCLASS()
class TCOS_MULTIPLAYER_API UDodgeRollAbility : public UTCOSGameplayAbility
{
	GENERATED_BODY()
	

public:
	UDodgeRollAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_ComboCount)
	int32 ComboCount = 0;

	UFUNCTION()
	void OnRep_ComboCount();


	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	float ComboResetTime = 1.5f;

	FTimerHandle ComboResetTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TArray<TSoftObjectPtr<UAnimMontage>> ComboMontages;

	void PlayComboMontage(ACharacterBase* Character);

	void ResetCombo();
};
