// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DSGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDSGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UDSGameInstanceSubsystem();
	void InitGameLift(const FServerParameters& ServerParams);

	UPROPERTY(BlueprintReadOnly)
	bool bGameLiftInitialized;

private:
	void ParseCommandLinePort(int32& OutPort);

	FProcessParameters ProcessParameters;
	
	
};
