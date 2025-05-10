// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/MeleeAttackAbility.h"
#include "Character/TCOS_MultiplayerCharacter.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Net/UnrealNetwork.h"

UMeleeAttackAbility::UMeleeAttackAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UMeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    UE_LOG(LogTemp, Warning, TEXT("ActivateAbility ServerPerformAttack"));
    if (!HasAuthority(&ActivationInfo)) return; 
    ATCOS_MultiplayerCharacter* Character = Cast<ATCOS_MultiplayerCharacter>(ActorInfo->AvatarActor.Get());
    if (!Character) return;

	FHitResult HitResult;
	Character->CombatComponent->ServerPerformAttack(HitResult.ImpactPoint);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UMeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
