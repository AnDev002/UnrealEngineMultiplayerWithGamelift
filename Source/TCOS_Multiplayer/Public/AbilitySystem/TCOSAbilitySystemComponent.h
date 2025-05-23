// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "TCOSAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttributesGiven);
/**
 * 
 */
UCLASS()
class TCOS_MULTIPLAYER_API UTCOSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	FOnAttributesGiven OnAttributesGiven;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassivesToGrant);
	void InitializeDefaultAttributes(const TSubclassOf<UGameplayEffect>& AttributeEffect);
	
	void AbilityInputPressed(FGameplayTag& InputTag);
	void AbilityInputReleased(FGameplayTag& InputTag);


};
