// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/TCOSAbilitySystemLibrary.h"
#include "DataAssets/CharacterClassInfo.h"
#include "Game/GameMode/TCOSGameMode.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

UCharacterClassInfo* UTCOSAbilitySystemLibrary::GetCharacterClassDefaultInfo(const UObject* WorldContextObject)
{
	if (ATCOSGameMode* TCOSGameMode = Cast<ATCOSGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return TCOSGameMode->GetCharacterClassDefaultInfo();
	}
	return nullptr;
}

void UTCOSAbilitySystemLibrary::ApplyDamageEffectInfo(const FDamageEffectInfo& DamageEffectInfo)
{
	FGameplayEffectContextHandle ContextHandle = DamageEffectInfo.SourceASC->MakeEffectContext();
	if (IsValid(DamageEffectInfo.AvatarActor))
	{
		ContextHandle.AddSourceObject(DamageEffectInfo.AvatarActor);

		if (IsValid(DamageEffectInfo.DamageEffect))
		{
			const FGameplayEffectSpecHandle SpecHandle = DamageEffectInfo.SourceASC->MakeOutgoingSpec(DamageEffectInfo.DamageEffect, DamageEffectInfo.AbilityLevel, ContextHandle);
			if (IsValid(DamageEffectInfo.TargetASC))
			{
				DamageEffectInfo.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}

	}
	
}
