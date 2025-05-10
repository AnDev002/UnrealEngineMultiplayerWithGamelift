// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/TCOS_MultiplayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Game/PlayerState/TCOSPlayerState.h"
#include "AbilitySystem/Attributes/TCOSAttributeSet.h"
#include "AbilitySystem/TCOSAbilitySystemComponent.h"
#include "Libraries/TCOSAbilitySystemLibrary.h"
#include "DataAssets/CharacterClassInfo.h"
#include "AbilitySystem/Attributes/TCOSAttributeSet.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Weapon/BaseWeapon.h"
#include "AbilitySystem/Abilities/BaseAttackAbility.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/BaseWeapon.h"
#include "Animation/AnimMontage.h"
#include "DataAssets/WeaponInfo.h"
#include "AbilitySystem/Abilities/TCOSDamageAbility.h"
#include "AbilitySystem/Abilities/HitReactionAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/VFX/UTCOSVFXComponent.h"
#include "Game/PlayerController/TCOSPlayerController.h"
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATCOS_MultiplayerCharacter

ATCOS_MultiplayerCharacter::ATCOS_MultiplayerCharacter()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	GetMesh()->SetIsReplicated(true);
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("CombatComponent"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	bIsAttacking = false;

	VFXComponent = CreateDefaultSubobject<UUTCOSVFXComponent>(TEXT("VFXComponent"));
	VFXComponent->SetIsReplicated(true);


	UE_LOG(LogTemp, Warning, TEXT("Spawned actor: %s | Class: %s | bReplicates: %d | HasAuthority: %d"),
		*GetName(),
		*GetClass()->GetName(),
		bReplicates,
		HasAuthority());

	bEnableGameActions = true;
	bQuitMenuOpen = false;
}

void ATCOS_MultiplayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		InitAbilityActorInfo();
	}
}

void ATCOS_MultiplayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

UAbilitySystemComponent* ATCOS_MultiplayerCharacter::GetAbilitySystemComponent() const
{
	return TCOSAbilitySystemComp;
}

void ATCOS_MultiplayerCharacter::SetOverlappingWeapon(ABaseWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ATCOS_MultiplayerCharacter::OnRep_OverlappingWeapon(ABaseWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ATCOS_MultiplayerCharacter::EquipButtonPressed()
{
	if (CombatComponent && OverlappingWeapon)
	{
		if (HasAuthority())
		{
			CombatComponent->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}

		TSubclassOf<UGameplayAbility> ReactionAbility = GetEquippedWeapon()->WeaponData->HitReactionAbility;
		FGameplayAbilitySpec Spec(ReactionAbility, 1, INDEX_NONE, this);
		TCOSAbilitySystemComp->GiveAbility(Spec);
	}
}

void ATCOS_MultiplayerCharacter::ServerEquipButtonPressed_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
}

void ATCOS_MultiplayerCharacter::AttackButtonPressed()
{
	if (CombatComponent)
	{
		if (!bIsAttacking)
		{
			CombatComponent->Attack();
			bIsAttacking = true;
		}
		else
		{
			CombatComponent->bWantsToCombo = true;
		}
	}
}

void ATCOS_MultiplayerCharacter::PlayAttackMontage()
{
	if (CombatComponent)
	{
		if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr) return;

		//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//if (!AnimInstance) return;
		//
		////bIsAttacking = true;
		////UAnimMontage* AttackMontage = CombatComponent->EquippedWeapon->WeaponData->ComboMontages[0].LoadSynchronous();
		//if (SingleSwordMontage)
		//{
		//	AnimInstance->Montage_Play(SingleSwordMontage);
		//}
		PlayAnimation(SingleSwordMontage);
	}
}


void ATCOS_MultiplayerCharacter::PerformLinetrace()
{
	//if (CombatComponent->EquippedWeapon->WeaponType != EWeaponType::SINGLE_SWORD) return;
	FVector StartLocation = CombatComponent->EquippedWeapon->GetWeaponMesh()->GetSocketLocation(FName("StartSocket"));
	FVector EndLocation = CombatComponent->EquippedWeapon->GetWeaponMesh()->GetSocketLocation(FName("EndSocket"));

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Pawn, TraceParams);


	if (HitResult.bBlockingHit)
	{
		ATCOS_MultiplayerCharacter* ActorHit = Cast<ATCOS_MultiplayerCharacter>(HitResult.GetActor());
		if (ActorHit)
		{
			UTCOSAbilitySystemComponent* HitActorAbilitySystem = ActorHit->GetTCOSAbilitySystemComp();
			if (HitActorAbilitySystem)
			{
				EWeaponType WeaponType = CombatComponent->EquippedWeapon->WeaponType;

				TSubclassOf<UGameplayEffect> DamageEffect = CombatComponent->EquippedWeapon->WeaponData->DamageEffect;

				if (DamageEffect)
				{
					FGameplayEffectContextHandle ContextHandle = HitActorAbilitySystem->MakeEffectContext();
					FGameplayEffectSpecHandle EffectSpecHandle = HitActorAbilitySystem->MakeOutgoingSpec(DamageEffect, 1.0f, ContextHandle);

					if (EffectSpecHandle.IsValid())
					{
						HitActorAbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
						ActorHit->MulticastHandleHitReaction(this);

						FVector StartSLocation = CombatComponent->EquippedWeapon->GetWeaponMesh()->GetSocketLocation(FName("StartSocket"));
						FVector EndSLocation = CombatComponent->EquippedWeapon->GetWeaponMesh()->GetSocketLocation(FName("EndSocket"));
					
						FVector BloodLocation = HitResult.ImpactPoint;
						FRotator BloodRotation = (EndSLocation - StartSLocation).Rotation();
						ActorHit->MulticastSpawnEffect(ActorHit->VFXComponent->BloodEffect, BloodLocation, BloodRotation, FVector(0.1f));
					}
				}
			}
		}
	}
}

void ATCOS_MultiplayerCharacter::HandleCameraZoomOut()
{
	if (CameraBoom && !GetWorld()->GetTimerManager().IsTimerActive(ZoomTimerHandle))
	{
		float TargetArmLength = 800.f;
		FVector TargetSocketOffset = FVector(0.f, 0.f, 150.f);
		FRotator TargetRotation = FRotator(-25.f, 0.f, 0.f);

		float CurrentLength = CameraBoom->TargetArmLength;
		FVector CurrentOffset = CameraBoom->SocketOffset;
		FRotator CurrentRotation = CameraBoom->GetRelativeRotation();

		float Duration = 3.0f;
		float StepTime = 0.01f;
		int32 Steps = Duration / StepTime;
		int32 StepCounter = 0;

		GetWorld()->GetTimerManager().SetTimer(ZoomTimerHandle,
			[this, CurrentLength, TargetArmLength, CurrentOffset, TargetSocketOffset, CurrentRotation, TargetRotation, Steps]() mutable
			{
				static int32 StepCounter = 0;

				float AlphaLinear = static_cast<float>(StepCounter) / Steps;
				float Alpha = FMath::InterpEaseOut(0.f, 1.f, AlphaLinear, 3.f);

				CameraBoom->TargetArmLength = FMath::Lerp(CurrentLength, TargetArmLength, Alpha);
				CameraBoom->SocketOffset = FMath::Lerp(CurrentOffset, TargetSocketOffset, Alpha);
				CameraBoom->SetRelativeRotation(FMath::Lerp(CurrentRotation, TargetRotation, Alpha)
				);

				StepCounter++;

				if (StepCounter > Steps)
				{
					GetWorld()->GetTimerManager().ClearTimer(ZoomTimerHandle);
					StepCounter = 0;
					ResetCameraZoom();
				}
			},
			StepTime, true
		);
	}
}


void ATCOS_MultiplayerCharacter::ResetCameraZoom()
{
	if (CameraBoom)
	{
		if (CameraBoom && !GetWorld()->GetTimerManager().IsTimerActive(ZoomTimerHandle))
		{
			float TargetArmLength = 400.f;
			FVector TargetSocketOffset = FVector(0.f, 0.f, 50.f);
			FRotator TargetRotation = FRotator(-10.f, 0.f, 0.f);

			float CurrentLength = CameraBoom->TargetArmLength;
			FVector CurrentOffset = CameraBoom->SocketOffset;
			FRotator CurrentRotation = CameraBoom->GetRelativeRotation();

			float Duration = 1.5f; 
			float StepTime = 0.01f;
			int32 Steps = Duration / StepTime;
			int32 StepCounter = 0;

			GetWorld()->GetTimerManager().SetTimer(ZoomTimerHandle,
				[this, CurrentLength, TargetArmLength, CurrentOffset, TargetSocketOffset, CurrentRotation, TargetRotation, Steps, StepCounter]() mutable
				{
					float AlphaLinear = static_cast<float>(StepCounter) / Steps;
					float Alpha = FMath::InterpEaseIn(0.f, 1.f, AlphaLinear, 2.5f); 

					CameraBoom->TargetArmLength = FMath::Lerp(CurrentLength, TargetArmLength, Alpha);
					CameraBoom->SocketOffset = FMath::Lerp(CurrentOffset, TargetSocketOffset, Alpha);
					CameraBoom->SetRelativeRotation(FMath::Lerp(CurrentRotation, TargetRotation, Alpha));

					StepCounter++;

					if (StepCounter > Steps)
					{
						GetWorld()->GetTimerManager().ClearTimer(ZoomTimerHandle);
					}
				},
				StepTime, true
			);
		}
	}
}

void ATCOS_MultiplayerCharacter::EnableGameActions_Implementation(bool bEnable)
{
	bEnableGameActions = bEnable;
	//if (!bEnable && IsValid(Combat))
	//{
	//	Combat->Initiate_FireWeapon_Released();
	//}
}

void ATCOS_MultiplayerCharacter::OnRep_RightHandWeapon()
{
	Super::OnRep_RightHandWeapon();
}

void ATCOS_MultiplayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATCOS_MultiplayerCharacter, RightHandWeapon);
	DOREPLIFETIME_CONDITION(ATCOS_MultiplayerCharacter, OverlappingWeapon, COND_AutonomousOnly);

}
//void ATCOS_MultiplayerCharacter::OnHealthChanged(float CurrentHealth, float MaxHealth)
//{
//}
//
//void ATCOS_MultiplayerCharacter::OnStaminaChanged(float CurrentStamina, float MaxStamina)
//{
//}

//////////////////////////////////////////////////////////////////////////
// Input

void ATCOS_MultiplayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATCOS_MultiplayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATCOS_MultiplayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATCOS_MultiplayerCharacter::Look);
		
		// Pickup
		//EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Started, this, &ATCOS_MultiplayerCharacter::Look);

		EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Started, this, &ATCOS_MultiplayerCharacter::Input_Quit);


	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ATCOS_MultiplayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		InitAbilityActorInfo();
		Server_ApplyDefaultEffects();
	}
	/*if (listWeapons.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, listWeapons.Num() - 1);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnTransform = FTransform::Identity; 
		listWeapons[RandomIndex]->GetDefaultObject<AActor>()->bNetStartup = false;
		ABaseWeapon* RandomWeapon = GetWorld()->SpawnActor<ABaseWeapon>(
			listWeapons[RandomIndex],
			SpawnTransform,
			SpawnParams
		);
		RandomWeapon->SetReplicates(true);

		OverlappingWeapon = RandomWeapon;
		CombatComponent->EquippedWeapon = RandomWeapon;
		if (RandomWeapon)
		{
			CombatComponent->EquipWeapon(OverlappingWeapon);
		}
	}*/
}

void ATCOS_MultiplayerCharacter::Server_ApplyDefaultEffects_Implementation()
{
	if (HasAuthority())
	{
		InitClassDefaults();

		if (TCOSAbilitySystemComp && StaminaRegen)
		{
			FGameplayEffectContextHandle ContextHandle = TCOSAbilitySystemComp->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = TCOSAbilitySystemComp->MakeOutgoingSpec(StaminaRegen, 1.0f, ContextHandle);

			if (SpecHandle.IsValid())
			{
				TCOSAbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}


void ATCOS_MultiplayerCharacter::InitAbilityActorInfo()
{
	if (ATCOSPlayerState* TCOSPlayerState = GetPlayerState<ATCOSPlayerState>())
	{
		TCOSAbilitySystemComp = TCOSPlayerState->GetTCOSAbilitySystemComponent();
		TCOSAttributes = TCOSPlayerState->GetTCOSAttribute();

		if (IsValid(TCOSAbilitySystemComp))
		{
			TCOSAbilitySystemComp->InitAbilityActorInfo(TCOSPlayerState, this);
			BindCallbacksToDependencies();

			if (HasAuthority())
			{
				InitClassDefaults();


				if (TCOSAbilitySystemComp && StaminaRegen)
				{
					FGameplayEffectContextHandle ContextHandle = TCOSAbilitySystemComp->MakeEffectContext();
					FGameplayEffectSpecHandle SpecHandle = TCOSAbilitySystemComp->MakeOutgoingSpec(StaminaRegen, 1.0f, ContextHandle);

					if (SpecHandle.IsValid())
					{
						TCOSAbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
					}
				}
			}
		}
	}
}

void ATCOS_MultiplayerCharacter::InitClassDefaults()
{
	if (!CharacterTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Character Tag Selected In This Character %s"), *GetNameSafe(this));
	}
	else if (UCharacterClassInfo* ClassInfo = UTCOSAbilitySystemLibrary::GetCharacterClassDefaultInfo(this))
	{
		if (const FCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag))
		{
			if (IsValid(TCOSAbilitySystemComp))
			{
				TCOSAbilitySystemComp->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
				TCOSAbilitySystemComp->AddCharacterPassiveAbilities(SelectedClassInfo->StartingPassives);
				TCOSAbilitySystemComp->InitializeDefaultAttributes(SelectedClassInfo->DefaultAttributes);
			}
		}
	}
}

void ATCOS_MultiplayerCharacter::BindCallbacksToDependencies()
{
	if (IsValid(TCOSAbilitySystemComp) && IsValid(TCOSAttributes))
	{
		TCOSAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(TCOSAttributes->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged(Data.NewValue, TCOSAttributes->GetMaxHealth());
			});

		TCOSAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(TCOSAttributes->GetStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnStaminaChanged(Data.NewValue, TCOSAttributes->GetMaxStamina());
			});
	}
}

void ATCOS_MultiplayerCharacter::BroadcastInitialValue()
{
	if (IsValid(TCOSAttributes))
	{
		OnHealthChanged(TCOSAttributes->GetHealth(), TCOSAttributes->GetMaxHealth());
		OnStaminaChanged(TCOSAttributes->GetStamina(), TCOSAttributes->GetMaxStamina());
	}
}



void ATCOS_MultiplayerCharacter::Input_Quit()
{
	bQuitMenuOpen = !bQuitMenuOpen;
	ATCOSPlayerController* PlayerController = Cast<ATCOSPlayerController>(Controller);
	if (bQuitMenuOpen)
	{
		FInputModeGameAndUI InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
		PlayerController->OnQuitMenuOpen.Broadcast(true);
	}
	else
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
		PlayerController->OnQuitMenuOpen.Broadcast(false);
	}
}

void ATCOS_MultiplayerCharacter::HandleHitReaction(AActor* DamageCauser)
{
	if (!TCOSAbilitySystemComp) return;

	ACharacterBase* Victim = this;
	ABaseWeapon* VictimWeapon = Victim->GetEquippedWeapon();
	if (!VictimWeapon) return;

	TSubclassOf<UGameplayAbility> ReactionAbility = VictimWeapon->WeaponData->HitReactionAbility;
	if (!ReactionAbility) return;
	FGameplayEventData EventData;
	EventData.Instigator = DamageCauser;
	EventData.Target = this;

	TCOSAbilitySystemComp->HandleGameplayEvent(
		FGameplayTag::RequestGameplayTag(FName("Ability.HitReaction")),
		&EventData);
}

void ATCOS_MultiplayerCharacter::MulticastHandleHitReaction_Implementation(AActor* DamageCauser)
{
	HandleHitReaction(DamageCauser);
}

void ATCOS_MultiplayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATCOS_MultiplayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
