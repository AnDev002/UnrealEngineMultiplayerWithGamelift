// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/TCOSAbilityTypes.h"
#include "TCOSAbilitySystemLibrary.generated.h"

class UCharacterClassInfo;

UCLASS()
class TCOS_MULTIPLAYER_API UTCOSAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure)
	static UCharacterClassInfo* GetCharacterClassDefaultInfo(const UObject* WorldContextObject);
	
	template<typename T>
	static T* GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable)
	static void ApplyDamageEffectInfo(const FDamageEffectInfo& DamageEffectInfo);
	
};

template<typename T>
inline T* UTCOSAbilitySystemLibrary::GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag)
{

	return DataTable->FindRow<T>(Tag.GetTagName(), FString(""));
}
