// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Player/DSMatchPlayerState.h"
#include "TCOSPlayerState.generated.h"

class UTCOSAbilitySystemComponent;
class UTCOSAttributeSet;

UCLASS()
class TCOS_MULTIPLAYER_API ATCOSPlayerState : public ADSMatchPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	ATCOSPlayerState();
	virtual void OnMatchEnded(const FString& Username) override;
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


	int32 Score;
	int32 Defeats;
	int32 Hits;

	int32 HighestStreak;
	bool bFirstBlood;
	bool bWinner;

};
