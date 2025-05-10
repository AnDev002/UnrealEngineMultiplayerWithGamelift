// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VFX/UTCOSVFXComponent.h"
#include "NiagaraFunctionLibrary.h"

UUTCOSVFXComponent::UUTCOSVFXComponent()
{
	SetIsReplicatedByDefault(true);
}

void UUTCOSVFXComponent::SpawnBloodEffect(FVector Location, FRotator Rotation)
{
	if (BloodEffect)
	{
		MulticastSpawnEffect(BloodEffect, Location, Rotation);
	}
}

void UUTCOSVFXComponent::SpawnHitEffect(FVector Location, FRotator Rotation)
{
	if (HitEffect)
	{
		MulticastSpawnEffect(HitEffect, Location, Rotation);
	}
}

void UUTCOSVFXComponent::MulticastSpawnEffect_Implementation(UNiagaraSystem* Effect, FVector Location, FRotator Rotation)
{
	UE_LOG(LogTemp, Warning, TEXT("MulticastSpawnEffect Called! Role: %s"), *UEnum::GetValueAsString(GetOwner()->GetLocalRole()));

	if (Effect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			Effect,
			Location,
			Rotation,
			FVector(1.0f),		// Scale
			true,				// AutoDestroy
			true,
			ENCPoolMethod::AutoRelease
		);
	}
}
