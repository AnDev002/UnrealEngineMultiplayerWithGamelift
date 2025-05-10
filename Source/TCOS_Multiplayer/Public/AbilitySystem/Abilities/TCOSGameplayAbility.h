// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "TCOSGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TCOS_MULTIPLAYER_API UTCOSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Input")
	FGameplayTag InputTag;
	

};
