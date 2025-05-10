
#include "UI/Lobby/LobbyPlayerBox.h"

#include "Components/ScrollBox.h"
#include "Game/DSGameState.h"
#include "Lobby/LobbyState.h"
#include "Lobby/LobbyPlayerInfo.h"
#include "UI/Lobby/PlayerLabel.h"

void ULobbyPlayerBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ADSGameState* DSGameState = GetWorld()->GetGameState<ADSGameState>();
	if (!IsValid(DSGameState)) return;

	if (IsValid(DSGameState->LobbyState))
	{
		OnLobbyStateInitialized(DSGameState->LobbyState);
	}
	else
	{
		DSGameState->OnLobbyStateInitialized.AddDynamic(this, &ULobbyPlayerBox::OnLobbyStateInitialized);
	}
}

void ULobbyPlayerBox::UpdatePlayerInfo(ALobbyState* LobbyState)
{
	ScrollBox_PlayerInfo->ClearChildren();
	for (const FLobbyPlayerInfo& PlayerInfo : LobbyState->GetPlayers())
	{
		CreateAndAddPlayerLabel(PlayerInfo);
	}
}

void ULobbyPlayerBox::OnLobbyStateInitialized(ALobbyState* LobbyState)
{
	if (!IsValid(LobbyState)) return;

	LobbyState->OnPlayerInfoAdded.AddDynamic(this, &ULobbyPlayerBox::CreateAndAddPlayerLabel);
	LobbyState->OnPlayerInfoRemoved.AddDynamic(this, &ULobbyPlayerBox::OnPlayerRemoved);
	UpdatePlayerInfo(LobbyState);
}

void ULobbyPlayerBox::CreateAndAddPlayerLabel(const FLobbyPlayerInfo& PlayerInfo)
{
	UPlayerLabel* PlayerLabel = CreateWidget<UPlayerLabel>(this, PlayerLabelClass);
	if (!IsValid(PlayerLabel)) return;

	PlayerLabel->SetUsername(PlayerInfo.Username);
	ScrollBox_PlayerInfo->AddChild(PlayerLabel);
}

void ULobbyPlayerBox::OnPlayerRemoved(const FLobbyPlayerInfo& PlayerInfo)
{
	if (UPlayerLabel* PlayerLabel = FindPlayerLabel(PlayerInfo.Username))
	{
		ScrollBox_PlayerInfo->RemoveChild(PlayerLabel);
	}
}

UPlayerLabel* ULobbyPlayerBox::FindPlayerLabel(const FString& Username)
{
	for (UWidget* Child : ScrollBox_PlayerInfo->GetAllChildren())
	{
		UPlayerLabel* PlayerLabel = Cast<UPlayerLabel>(Child);
		if (IsValid(PlayerLabel) && PlayerLabel->GetUsername() == Username)
		{
			return PlayerLabel;
		}
	}
	return nullptr;
}