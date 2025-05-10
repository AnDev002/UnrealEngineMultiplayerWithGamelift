#include "Lobby/LobbyPlayerInfo.h"

void FLobbyPlayerInfoArray::AddPlayer(const FLobbyPlayerInfo& NewPlayerInfo)
{
	int32 Index = Players.Add(NewPlayerInfo);
	MarkItemDirty(Players[Index]);
	Players[Index].PostReplicatedAdd(*this);
}

void FLobbyPlayerInfoArray::RemovePlayer(const FString& Username)
{
	for (int32 PlayerIndex = 0; PlayerIndex < Players.Num(); ++PlayerIndex)
	{
		FLobbyPlayerInfo& PlayerInfo = Players[PlayerIndex];
		if (PlayerInfo.Username == Username)
		{
			PlayerInfo.PreReplicatedRemove(*this);
			Players.RemoveAtSwap(PlayerIndex);
			MarkArrayDirty();
			break;
		}
	}
}