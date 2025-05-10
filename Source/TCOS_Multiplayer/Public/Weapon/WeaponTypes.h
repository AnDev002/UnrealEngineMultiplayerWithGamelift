// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	SINGLE_SWORD UMETA(DisplayName = "Single Sword"),
	DOUBLE_SWORD UMETA(DisplayName = "Double Sword"),
	DAGGER UMETA(DisplayName = "Dagger"),
	STAFF UMETA(DisplayName = "Staff"),
	AXE UMETA(DisplayName = "Axe"),
	BOW UMETA(DisplayName = "Bow"),
};

UCLASS()
class TCOS_MULTIPLAYER_API AWeaponTypes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponTypes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
