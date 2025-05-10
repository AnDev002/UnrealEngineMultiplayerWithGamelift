// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DodgeRollAbility.h"
#include "Character/CharacterBase.h"

#include "Net/UnrealNetwork.h"
UDodgeRollAbility::UDodgeRollAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UDodgeRollAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!HasAuthority(&ActivationInfo)) return;

    ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
    if (!Character) return;

    ComboCount = FMath::Min(ComboCount + 1, ComboMontages.Num());

    PlayComboMontage(Character);

    GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &UDodgeRollAbility::ResetCombo, ComboResetTime, false);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UDodgeRollAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDodgeRollAbility::OnRep_ComboCount()
{
    UE_LOG(LogTemp, Log, TEXT("Combo Count synced: %d"), ComboCount);
}

void UDodgeRollAbility::PlayComboMontage(ACharacterBase* Character)
{
    UE_LOG(LogTemp, Log, TEXT("PlayComboMontage UDodgeRollAbility"));

    if (ComboMontages.IsValidIndex(ComboCount - 1))
    {
        UAnimMontage* MontageToPlay = ComboMontages[ComboCount - 1].LoadSynchronous();
        if (MontageToPlay && Character->GetMesh()->GetAnimInstance())
        {
            Character->MulticastPlayComboMontage(MontageToPlay);
            UE_LOG(LogTemp, Log, TEXT("Playing combo %d"), ComboCount);
        }
    }
}

void UDodgeRollAbility::ResetCombo()
{
    ComboCount = 0;
}

void UDodgeRollAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UDodgeRollAbility, ComboCount);
}