// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DSMatchGameMode.h"
#include "TCOSGameMode.generated.h"

class UCharacterClassInfo;
class UMeleeWeaponInfo;

UCLASS()
class TCOS_MULTIPLAYER_API ATCOSGameMode : public ADSMatchGameMode
{
	GENERATED_BODY()
	
public:
	UCharacterClassInfo* GetCharacterClassDefaultInfo() const;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	void FindPlayerStartTransform(AController* Player, FVector& OutLocation, FRotator& OutRotation);
private:
	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Class Defaults")
	TObjectPtr<UCharacterClassInfo> ClassDefaults;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Class Defaults")
	TObjectPtr<UMeleeWeaponInfo> MeleeWeaponInfo;

protected:
	ATCOSGameMode();
   
};
