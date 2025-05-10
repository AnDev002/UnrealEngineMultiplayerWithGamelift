// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/HeroCombatComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Character/TCOS_MultiplayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PlayerController/TCOSPlayerController.h"
#include "DataAssets/WeaponInfo.h"
#include "AbilitySystem/TCOSAbilityTypes.h"
#include "AbilitySystem/Abilities/TCOSDamageAbility.h"
#include "Weapon/Melee/MeleeWeaponBase.h"
#include "AbilitySystem/TCOSAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"



UHeroCombatComponent::UHeroCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UHeroCombatComponent::BeginPlay()
{
	BaseWalkSpeed = 600.f;
	/*BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;*/
	Super::BeginPlay();

	Character = Cast<ATCOS_MultiplayerCharacter>(GetOwner());
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void UHeroCombatComponent::MulticastZoomOutCamera_Implementation()
{
	if (Character && Character->IsLocallyControlled())
	{
		Character->HandleCameraZoomOut(); 
	}
}

void UHeroCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHeroCombatComponent::EquipWeapon(ABaseWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	const USkeletalMeshSocket* WeaponSocket = Character->GetMesh()->GetSocketByName(FName(WeaponToEquip->WeaponData->WeaponSocketName));

	if (WeaponSocket)
	{
		WeaponSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);
	/*Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;*/
}

void UHeroCombatComponent::Attack()
{
	if (!Character || !EquippedWeapon) return;

	if (Character && Character->GetController() && Character->GetController()->IsLocalController())
	{
		FHitResult HitResult;
		ServerPerformAttack(HitResult.ImpactPoint);
	}
}

void UHeroCombatComponent::ServerPerformAttack_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (!bIsAttacking)
	{
		if (!Character || !EquippedWeapon || !EquippedWeapon->WeaponData) return;

		TArray<TSoftObjectPtr<UAnimMontage>>& ComboMontages = EquippedWeapon->WeaponData->ComboMontages;
		AttackComboCount++;
	
		MulticastPlayAttackMontage(TraceHitTarget, AttackComboCount);
		if (AttackComboCount >= ComboMontages.Num())
		{
			ResetComboCount();
			if (EquippedWeapon->WeaponData->WeaponSocketName == "RKnifeSocket")
			{
				MulticastZoomOutCamera();
			}
		}
		GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &UHeroCombatComponent::ResetComboCount, 3.0f, false);
	}
	else
	{
		bWantsToCombo = true;
	}
}

void UHeroCombatComponent::TryContinueCombo()
{
	if (bWantsToCombo)
	{
		//TArray<TSoftObjectPtr<UAnimMontage>>& ComboMontages = EquippedWeapon->WeaponData->ComboMontages;
		//AttackComboCount = FMath::Clamp(AttackComboCount + 1, 1, ComboMontages.Num());
		//Attack();
		bWantsToCombo = false;
	}
}

void UHeroCombatComponent::MulticastPlayAttackMontage_Implementation(const FVector_NetQuantize& TraceHitTarget, int AttackCount)
{
	if (Character && EquippedWeapon && EquippedWeapon->WeaponData)
	{
		TArray<TSoftObjectPtr<UAnimMontage>>& ComboMontages = EquippedWeapon->WeaponData->ComboMontages;
		int32 ComboIndex = AttackCount > 0 ? FMath::Min(AttackCount - 1, ComboMontages.Num() - 1) : 0;
		if (AttackCount <= 0) return;

		if (ComboMontages.IsValidIndex(ComboIndex))
		{
			UAnimMontage* AttackMontage = ComboMontages[ComboIndex].LoadSynchronous();
			if (AttackMontage)
			{
				Character->PlayAnimation(AttackMontage);
				bWantsToCombo = false;
				UTCOSAbilitySystemComponent* ActorAbilitySystem = Character->GetTCOSAbilitySystemComp();
				if (ActorAbilitySystem)
				{
					TSubclassOf<UGameplayEffect> StaminaOnAttackEffect = Character->CombatComponent->EquippedWeapon->WeaponData->DecreaseStaminaOnAttack;
					if (StaminaOnAttackEffect)
					{
						FGameplayEffectContextHandle ContextHandle = ActorAbilitySystem->MakeEffectContext();
						FGameplayEffectSpecHandle EffectSpecHandle = ActorAbilitySystem->MakeOutgoingSpec(StaminaOnAttackEffect, 1.0f, ContextHandle);

						if (EffectSpecHandle.IsValid())
						{
							ActorAbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
						}
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to load montage at index %d"), ComboIndex);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ComboMontages index %d out of range"), ComboIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Character, EquippedWeapon, or WeaponData is null"));
	}
}

void UHeroCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character) {
		//Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		//Character->bUseControllerRotationYaw = true;
	}
}

void UHeroCombatComponent::OnRep_RightComboCount()
{
	UE_LOG(LogTemp, Warning, TEXT("ComboCount synced"));
}

void UHeroCombatComponent::ResetComboCount()
{
	AttackComboCount = 0;
	bWantsToCombo = false;
	UE_LOG(LogTemp, Warning, TEXT("Combo reset"));
}

void UHeroCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHeroCombatComponent, EquippedWeapon);
	//DOREPLIFETIME(UHeroCombatComponent, AttackComboCount);
}