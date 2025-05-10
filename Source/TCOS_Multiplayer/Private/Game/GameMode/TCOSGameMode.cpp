// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameMode/TCOSGameMode.h"
#include "DataAssets/CharacterClassInfo.h"
#include "DataAssets/MeleeWeaponInfo.h"

UCharacterClassInfo* ATCOSGameMode::GetCharacterClassDefaultInfo() const
{
	return ClassDefaults;
}

ATCOSGameMode::ATCOSGameMode()
{
    bReplicates = true;
    bAlwaysRelevant = true;
	bUseSeamlessTravel = true;
}

void ATCOSGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);
    if (NewPlayer->GetPawn() == nullptr)
    {
        FVector SpawnLocation;
        FRotator SpawnRotation;
        FindPlayerStartTransform(NewPlayer, SpawnLocation, SpawnRotation);
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = NewPlayer;
        AActor* TemplateActor = DefaultPawnClass->GetDefaultObject<AActor>();
        if (TemplateActor)
        {
            TemplateActor->bNetStartup = false;
        }
        APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (NewPawn)
        {
            NewPlayer->Possess(NewPawn);
        }
    }
}

void ATCOSGameMode::FindPlayerStartTransform(AController* Player, FVector& OutLocation, FRotator& OutRotation)
{
    AActor* StartSpot = FindPlayerStart(Player);
    if (StartSpot)
    {
        OutLocation = StartSpot->GetActorLocation();
        OutRotation = StartSpot->GetActorRotation();
    }
    else
    {
        OutLocation = FVector::ZeroVector;
        OutRotation = FRotator::ZeroRotator;
    }
}
