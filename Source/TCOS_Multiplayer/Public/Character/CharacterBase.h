#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/TCOSAbilityTypes.h"
#include "NiagaraSystem.h"
#include "CharacterBase.generated.h"

class UAnimMontage;
class UHeroCombatComponent;
class ABaseWeapon;

UCLASS()
class TCOS_MULTIPLAYER_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UHeroCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> StaminaRegen;
public:
	ACharacterBase();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStaminaChanged(float CurrentStamina, float MaxStamina);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayComboMontage(UAnimMontage* Montage);


	virtual void PlayAnimation(class UAnimMontage* Anim);

	UFUNCTION(BlueprintCallable)
	void Linetrace();

	virtual void PerformLinetrace();

	UFUNCTION(Server, Reliable)
	void ServerLinetrace();

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking;

	UFUNCTION()
	ABaseWeapon* GetEquippedWeapon();
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	virtual void BindCallbacksToDependencies();
	virtual void InitClassDefaults();

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValue();

	UPROPERTY(EditAnywhere, Category = "Custom Values|Character Info")
	FGameplayTag CharacterTag;

	FDamageEffectInfo DamageEffectInfo;

	UPROPERTY(ReplicatedUsing = OnRep_RightHandWeapon)
	ABaseWeapon* RightHandWeapon;

	UFUNCTION()
	virtual void OnRep_RightHandWeapon();


	UFUNCTION(NetMulticast, Unreliable)
	virtual void MulticastSpawnEffect(UNiagaraSystem* Effect, FVector Location, FRotator Rotation, FVector Scale);
};
