// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.h"
#include "Logging/LogMacros.h"
#include "Interfaces/PlayerInterface.h"
#include "AbilitySystem/TCOSAbilityTypes.h"
#include "TCOS_MultiplayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UTCOSAbilitySystemComponent;
class UTCOSAttributeSet;
class UHeroCombatComponent;
class ABaseWeapon;
class UUTCOSVFXComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATCOS_MultiplayerCharacter : public ACharacterBase, public IAbilitySystemInterface, public IPlayerInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickupAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuitAction;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UUTCOSVFXComponent* VFXComponent;

	UFUNCTION(Server, Reliable)
	void Server_ApplyDefaultEffects();
public:

	ATCOS_MultiplayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ABaseWeapon>> listWeapons;

	UFUNCTION(BlueprintCallable)
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void SetOverlappingWeapon(ABaseWeapon* Weapon);

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class ABaseWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(ABaseWeapon* LastWeapon);

	void EquipButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	void AttackButtonPressed();

	void PlayAttackMontage();

	//UPROPERTY(BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* SingleSwordMontage;

	virtual void PerformLinetrace() override;

	void HandleCameraZoomOut();
	void ResetCameraZoom();


	virtual void EnableGameActions_Implementation(bool bEnable) override;
protected:
	virtual void OnRep_RightHandWeapon() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	virtual void InitAbilityActorInfo() override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void BindCallbacksToDependencies() override;

	virtual void InitClassDefaults() override;
	virtual void BroadcastInitialValue() override;

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastPlayStaminaEffect();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UTCOSAbilitySystemComponent> TCOSAbilitySystemComp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UTCOSAttributeSet> TCOSAttributes;
		
	FGameplayAbilitySpecHandle HitReactionAbilityHandle;

	FTimerHandle ZoomTimerHandle;
	
	bool bEnableGameActions;

	void Input_Quit();
	bool bQuitMenuOpen;
public:
	TObjectPtr<UTCOSAbilitySystemComponent> GetTCOSAbilitySystemComp() const { return TCOSAbilitySystemComp; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void HandleHitReaction(AActor* DamageCauser);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleHitReaction(AActor* DamageCauser);

};

