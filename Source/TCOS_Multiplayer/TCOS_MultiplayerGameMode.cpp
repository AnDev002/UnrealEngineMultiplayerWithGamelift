// Copyright Epic Games, Inc. All Rights Reserved.

#include "TCOS_MultiplayerGameMode.h"
#include "Character/TCOS_MultiplayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATCOS_MultiplayerGameMode::ATCOS_MultiplayerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/Player/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
