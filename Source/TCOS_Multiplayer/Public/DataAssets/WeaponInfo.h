// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "WeaponInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;


UENUM(BlueprintType)
enum class EHitDirection : uint8
{
    EHD_Front UMETA(DisplayName = "Front"),
    EHD_Back UMETA(DisplayName = "Back"),
    EHD_Left UMETA(DisplayName = "Left"),
    EHD_Right UMETA(DisplayName = "Right")
};


/**
 * 
 */
UCLASS()
class TCOS_MULTIPLAYER_API UWeaponInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "Weapon")
    FString WeaponSocketName;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FGameplayTag WeaponType; // Sword, Axe, etc.

    UPROPERTY(EditAnywhere, Category = "Weapon")
    USkeletalMesh* WeaponMesh;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    TArray<TSoftObjectPtr<UAnimMontage>> ComboMontages;

    UPROPERTY(EditDefaultsOnly, Category = "Hit Reaction")
    TMap<EHitDirection, TSoftObjectPtr<UAnimMontage>> HitReactionMontages;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<UGameplayEffect> DamageEffect;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<UGameplayEffect> DecreaseStaminaOnAttack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Reaction")
    TSubclassOf<UGameplayAbility> HitReactionAbility;

};
