// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseAttackAbility.h"
#include "TCOSAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FDamageEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> AvatarActor = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffect = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;


};

USTRUCT()
struct FBaseAttackParams
{
	GENERATED_BODY()

	/*UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBaseAttackAbility> BaseAttackClass;*/
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> BaseMesh;

};

USTRUCT()
struct FMeleeAttackParams : public FBaseAttackParams
{
	GENERATED_BODY()


};
