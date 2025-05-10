// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TCOS_MeleeAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class TCOS_MULTIPLAYER_API UTCOS_MeleeAttackNotify : public UAnimNotifyState
{
	GENERATED_BODY()
	

public:
	virtual void NotifyBegin(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;
private:
	FTimerHandle TimerHandle;
};
