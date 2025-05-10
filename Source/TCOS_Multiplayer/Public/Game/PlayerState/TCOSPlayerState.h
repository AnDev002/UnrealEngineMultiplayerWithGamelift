// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TCOSPlayerState.generated.h"

class UTCOSAbilitySystemComponent;
class UTCOSAttributeSet;

UCLASS()
class TCOS_MULTIPLAYER_API ATCOSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	ATCOSPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure)
	UTCOSAbilitySystemComponent* GetTCOSAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure)
	UTCOSAttributeSet* GetTCOSAttribute() const;
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true)) 
	TObjectPtr<UTCOSAbilitySystemComponent> TCOSAbilitySystemComp;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true)) 
	TObjectPtr<UTCOSAttributeSet> TCOSAttributes;

};
