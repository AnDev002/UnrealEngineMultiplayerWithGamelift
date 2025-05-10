// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/TCOS_MeleeAttackNotify.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/CharacterBase.h"


void UTCOS_MeleeAttackNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner());
		if (Character)
		{
		}
	}
}

void UTCOS_MeleeAttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner());
		if (Character)
		{

		}
	}
}