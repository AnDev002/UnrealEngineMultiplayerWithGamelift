#include "Game/DSGameState.h"
#include "Net/UnrealNetwork.h"
#include "Lobby/LobbyState.h"

ADSGameState::ADSGameState()
{
	bReplicates = true;
}

void ADSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADSGameState, LobbyState);
}

void ADSGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CreateLobbyState();
		OnLobbyStateInitialized.Broadcast(LobbyState);
	}
}

void ADSGameState::CreateLobbyState()
{
	if (UWorld* World = GetWorld(); IsValid(World))
	{
		FActorSpawnParameters SpawnParams;
		LobbyState = World->SpawnActor<ALobbyState>(
			ALobbyState::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		if (IsValid(LobbyState))
		{
			LobbyState->SetOwner(this);
		}
	}
}

void ADSGameState::OnRep_LobbyState()
{
	OnLobbyStateInitialized.Broadcast(LobbyState);
}