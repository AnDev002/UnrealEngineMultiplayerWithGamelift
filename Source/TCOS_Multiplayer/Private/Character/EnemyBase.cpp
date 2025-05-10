// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBase.h"
#include "AbilitySystem/TCOSAbilitySystemComponent.h"
#include "DataAssets/CharacterClassInfo.h"
#include "AbilitySystem/Attributes/TCOSAttributeSet.h"
#include "Libraries/TCOSAbilitySystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AEnemyBase::AEnemyBase()
{
	bReplicates = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	TCOSAbilitySystemComp = CreateDefaultSubobject<UTCOSAbilitySystemComponent>("AbilitySystemComponent");
	TCOSAbilitySystemComp->SetIsReplicated(true);
	TCOSAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	TCOSAttributes = CreateDefaultSubobject<UTCOSAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const
{
	return TCOSAbilitySystemComp;
}

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyBase, bInitAttributes);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	BindCallbacksToDependencies();
	InitAbilityActorInfo();
}

void AEnemyBase::InitAbilityActorInfo()
{
	if (IsValid(TCOSAbilitySystemComp) && IsValid(TCOSAttributes))
	{
		TCOSAbilitySystemComp->InitAbilityActorInfo(this, this);
		if (HasAuthority())
		{
			InitClassDefaults();
			BroadcastInitialValue();
		}
	}
}

void AEnemyBase::InitClassDefaults()
{
	if (!CharacterTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Character Tag Selected In This Character %s"), *GetNameSafe(this));
	}
	else if (UCharacterClassInfo* ClassInfo = UTCOSAbilitySystemLibrary::GetCharacterClassDefaultInfo(this))
	{
		if (const FCharacterClassDefaultInfo* SelectedClass = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag))
		{
			if (IsValid(TCOSAbilitySystemComp))
			{
				TCOSAbilitySystemComp->AddCharacterAbilities(SelectedClass->StartingAbilities);
				TCOSAbilitySystemComp->AddCharacterPassiveAbilities(SelectedClass->StartingPassives);
				TCOSAbilitySystemComp->InitializeDefaultAttributes(SelectedClass->DefaultAttributes);
			}
		}
	}
}

void AEnemyBase::BindCallbacksToDependencies()
{
	if (IsValid(TCOSAbilitySystemComp) && IsValid(TCOSAttributes))
	{
		TCOSAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(TCOSAttributes->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged(Data.NewValue, TCOSAttributes->GetMaxHealth());
			});

		if (HasAuthority())
		{
			TCOSAbilitySystemComp->OnAttributesGiven.AddLambda(
				[this]
				{
					bInitAttributes = true;
				});
		}
	}
}

void AEnemyBase::BroadcastInitialValue()
{
	if (IsValid(TCOSAttributes))
	{
		OnHealthChanged(TCOSAttributes->GetHealth(), TCOSAttributes->GetMaxHealth());
	}
}

void AEnemyBase::OnRep_InitAttributes()
{
	BroadcastInitialValue();
}
