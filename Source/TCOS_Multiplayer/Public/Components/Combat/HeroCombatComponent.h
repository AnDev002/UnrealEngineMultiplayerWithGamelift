// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystem/TCOSAbilityTypes.h"
#include "HeroCombatComponent.generated.h"

class ABaseWeapon;
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TCOS_MULTIPLAYER_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	ABaseWeapon* EquippedWeapon;

public:
	friend class ATCOS_MultiplayerCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(class ABaseWeapon* WeaponToEquip);

	void Attack();

	UFUNCTION(Server, Reliable)
	void ServerPerformAttack(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAttackMontage(const FVector_NetQuantize& TraceHitTarget, int AttackCount);

	UFUNCTION(BlueprintCallable)
	void TryContinueCombo();

	bool bWantsToCombo = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking = false;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastZoomOutCamera();
protected:
	virtual void BeginPlay() override;

private:
	UHeroCombatComponent();
	class ATCOS_MultiplayerCharacter* Character;
	class AMyPlayerController* Controller;
	class APlayerHUD* HUD;

	UFUNCTION()
	void OnRep_EquippedWeapon();


	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	bool bAttackButtonPressed;

	UPROPERTY(ReplicatedUsing = OnRep_RightComboCount)
	int32 AttackComboCount = 0;

	

protected:
		UFUNCTION()
		void OnRep_RightComboCount();

		FTimerHandle ComboResetTimer;

		void ResetComboCount();
};
