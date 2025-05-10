// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/GamePage.h"
#include "UI/API/GameSessions/JoinGame.h"
#include "Components/Button.h"
#include "UI/GameSessions/GameSessionsManager.h"
void UGamePage::NativeConstruct()
{
	Super::NativeConstruct();

	GameSessionsManager = NewObject<UGameSessionsManager>(this, GameSessionsManagerClass);
	GameSessionsManager->BroadcastJoinGameSessionMessage.AddDynamic(JoinGameWidget, &UJoinGame::SetStatusMessage);

	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &UGamePage::JoinGameButtonClicked);
}

void UGamePage::JoinGameButtonClicked()
{
	JoinGameWidget->Button_JoinGame->SetIsEnabled(false);
	GameSessionsManager->JoinGameSession();
}
