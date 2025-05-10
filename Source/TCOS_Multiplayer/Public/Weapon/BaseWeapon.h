// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "BaseWeapon.generated.h"

class UWeaponInfo;
class UGameplayEffect;
class UTCOSAbilitySystemComponent;
enum class EWeaponType : uint8;
enum class EHitDirection : uint8;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_Max UMETA(DisplayName = "DefaultMAX")
};


UCLASS()
class TCOS_MULTIPLAYER_API ABaseWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	UWeaponInfo* WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType WeaponType;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

	TSoftObjectPtr<UAnimMontage> GetHitReactionMontageByDirection(EHitDirection Direction) const;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UTCOSAbilitySystemComponent* AbilitySystem;


	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateVisual();

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

public:
	void SetWeaponState(EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; };

	void ShowPickupWidget(bool bShowWidget);

};
