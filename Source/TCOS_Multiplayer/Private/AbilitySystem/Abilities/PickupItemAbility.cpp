// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PickupItemAbility.h"
#include "Character/TCOS_MultiplayerCharacter.h"
#include "Weapon/BaseWeapon.h"

void UPickupItemAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!HasAuthority(&ActivationInfo)) return;

    ATCOS_MultiplayerCharacter* Character = Cast<ATCOS_MultiplayerCharacter>(ActorInfo->AvatarActor.Get());
    if (!Character) {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }
    

    Character->EquipButtonPressed();
    
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
