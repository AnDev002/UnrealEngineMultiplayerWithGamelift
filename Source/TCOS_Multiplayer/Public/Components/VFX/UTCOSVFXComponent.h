// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "NiagaraSystem.h"
#include "UTCOSVFXComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TCOS_MULTIPLAYER_API UUTCOSVFXComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
	UUTCOSVFXComponent();
public:
	void SpawnBloodEffect(FVector Location, FRotator Rotation);
	void SpawnHitEffect(FVector Location, FRotator Rotation);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnEffect(UNiagaraSystem* Effect, FVector Location, FRotator Rotation);
	

public:
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* HitEffect;
	
};
