// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HitReactionAbility.h"
#include "DataAssets/WeaponInfo.h"
#include "Character/CharacterBase.h"
#include "Abilities/GameplayAbility.h"                 
#include "AbilitySystemComponent.h"                   
#include "AbilitySystemBlueprintLibrary.h"      
#include "GameplayTagContainer.h" 
#include "Weapon/BaseWeapon.h"
#include "Kismet/KismetMathLibrary.h"

UHitReactionAbility::UHitReactionAbility()
{
    GameplayEventTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.HitReaction")));
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.HitReaction")));

    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    FAbilityTriggerData TriggerData;
    TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(FName("Ability.HitReaction"));
    TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    AbilityTriggers.Add(TriggerData);
}

void UHitReactionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

    ACharacterBase* Victim = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
    AActor* Attacker = TriggerEventData ? const_cast<AActor*>(TriggerEventData->Instigator.Get()) : nullptr;

    if (!Victim) return;

    if (!Attacker) return;
    
    // 1. 
    EHitDirection HitDir = CalculateHitDirection(Victim, Attacker);
    FString HitDirName = UEnum::GetValueAsString(HitDir);
    UE_LOG(LogTemp, Warning, TEXT("Hit Direction: %s"), *HitDirName);
    // 2. 
    ABaseWeapon* Weapon = Victim->GetEquippedWeapon();
    TSoftObjectPtr<UAnimMontage> Montage = Weapon ? Weapon->GetHitReactionMontageByDirection(HitDir) : nullptr;

    FVector Forward = Victim->GetActorForwardVector().GetSafeNormal2D();
    FVector Right = Victim->GetActorRightVector().GetSafeNormal2D();
    FVector ToAttacker = (Attacker->GetActorLocation() - Victim->GetActorLocation()).GetSafeNormal2D();

    float ForwardDot = FVector::DotProduct(Forward, ToAttacker);
    float RightDot = FVector::DotProduct(Right, ToAttacker);

    UAnimMontage* ResolvedMontage = nullptr;
    if (Montage.IsValid())
    {
        if (Montage.IsValid())
        {
            ResolvedMontage = Montage.Get();
        }
        else
        {
            ResolvedMontage = Montage.LoadSynchronous();
        }
        if (ResolvedMontage)
        {
            Victim->MulticastPlayComboMontage(ResolvedMontage);
        }
    }
    else
    {
        UAnimMontage* LoadedMontage = Montage.LoadSynchronous();
        if (LoadedMontage)
        {
            if (Montage.IsValid())
            {
                ResolvedMontage = Montage.Get();
            }
            else
            {
                ResolvedMontage = Montage.LoadSynchronous();
            }
            if (ResolvedMontage)
            {
                Victim->MulticastPlayComboMontage(ResolvedMontage);
            }
        }
    }
}

void UHitReactionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

EHitDirection UHitReactionAbility::CalculateHitDirection(const AActor* Victim, const AActor* Attacker)
{
    if (!Victim || !Attacker)
        return EHitDirection::EHD_Front;

    FVector Direction = Attacker->GetActorLocation() - Victim->GetActorLocation();

    float VictimYaw = Victim->GetActorRotation().Yaw;
    float AttackerAngleFromVictim = UKismetMathLibrary::FindLookAtRotation(Victim->GetActorLocation(), Attacker->GetActorLocation()).Yaw;

    float DeltaYaw = FMath::FindDeltaAngleDegrees(VictimYaw, AttackerAngleFromVictim);

    UE_LOG(LogTemp, Warning, TEXT("Victim Yaw: %f, Attacker Angle: %f, DeltaYaw: %f"), VictimYaw, AttackerAngleFromVictim, DeltaYaw);

    if (DeltaYaw >= -45.f && DeltaYaw <= 45.f)
        return EHitDirection::EHD_Front;
    else if (DeltaYaw > 45.f && DeltaYaw <= 135.f)
        return EHitDirection::EHD_Right;
    else if (DeltaYaw < -45.f && DeltaYaw >= -135.f)
        return EHitDirection::EHD_Left;
    else
        return EHitDirection::EHD_Back;
}
