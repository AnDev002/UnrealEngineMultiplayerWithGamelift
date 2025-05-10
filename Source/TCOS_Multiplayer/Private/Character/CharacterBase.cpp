// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "Animation/AnimMontage.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Weapon/WeaponTypes.h"
#include "Weapon/BaseWeapon.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemGlobals.h"
#include "Libraries/TCOSAbilitySystemLibrary.h"
#include "DataAssets/WeaponInfo.h"
#include "AbilitySystem/TCOSAbilitySystemComponent.h"
#include "Character/TCOS_MultiplayerCharacter.h"
#include "NiagaraFunctionLibrary.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ACharacterBase::PlayAnimation(UAnimMontage* Anim)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !Anim) return;
	
	AnimInstance->Montage_Play(Anim);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::InitAbilityActorInfo()
{
	// Intentionally left blank for now.
}

void ACharacterBase::BindCallbacksToDependencies()
{
	// Intentionally left blank for now.
}

void ACharacterBase::InitClassDefaults()
{
	// Intentionally left blank for now.
}

void ACharacterBase::BroadcastInitialValue()
{
	// Intentionally left blank for now. 
}

void ACharacterBase::OnRep_RightHandWeapon()
{
	if (RightHandWeapon)
	{
		RightHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RSwordSocket"));
	}
}

ABaseWeapon* ACharacterBase::GetEquippedWeapon()
{
	return CombatComponent->EquippedWeapon;
}

void ACharacterBase::Linetrace()
{
	if (IsLocallyControlled())
	{
		if (HasAuthority())
		{
			PerformLinetrace();
		}
		else
		{
			ServerLinetrace();
		}
	}
}

void ACharacterBase::PerformLinetrace()
{
	// Intentionally left blank for now.
}

void ACharacterBase::ServerLinetrace_Implementation()
{
	PerformLinetrace();
}

void ACharacterBase::MulticastPlayComboMontage_Implementation(UAnimMontage* Montage)
{
	if (Montage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage);
	}
}

void ACharacterBase::MulticastSpawnEffect_Implementation(UNiagaraSystem* Effect, FVector Location, FRotator Rotation, FVector Scale)
{
	if (Effect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Location, Rotation, Scale);
	}
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
