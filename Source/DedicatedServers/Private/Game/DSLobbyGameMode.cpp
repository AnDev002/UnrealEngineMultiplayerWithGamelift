// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DSLobbyGameMode.h"
#include "Game/DSGameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DSPlayerController.h"
#include "Lobby/LobbyPlayerInfo.h"
#include "Lobby/LobbyState.h"
#include "Game/DSGameState.h"

ADSLobbyGameMode::ADSLobbyGameMode()
{
	bUseSeamlessTravel = true;
	LobbyStatus = ELobbyStatus::WaitingForPlayers;
	MinPlayers = 1;
	LobbyCountdownTimer.Type = ECountdownTimerType::LobbyCountdown;
}

void ADSLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
	{
		LobbyStatus = ELobbyStatus::CountdownToSeamlessTravel;
		StartCountdownTimer(LobbyCountdownTimer);
	}
}

void ADSLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitGameLift();
}

void ADSLobbyGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type);
	if (Type == ECountdownTimerType::LobbyCountdown)
	{
		StopCountdownTimer(LobbyCountdownTimer);
		LobbyStatus = ELobbyStatus::SeamlessTravelling;
		TrySeamlessTravel(MapToTravelTo);
	}
}

void ADSLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	CancelCountdown();
	RemovePlayerSession(Exiting);
	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::Logout for %s"), *Exiting->GetName());
	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
	{
		RemovePlayerInfoFromLobbyState(Exiting);
	}
}

void ADSLobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	CancelCountdown();
	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::InitSeamlessTravelPlayer for %s"), *NewController->GetName());

	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
	{
		AddPlayerInfoToLobbyState(NewController);
	}
}

void ADSLobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));
	TryAcceptPlayerSession(PlayerSessionId, Username, ErrorMessage);
}

FString ADSLobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	if (ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(NewPlayerController); IsValid(DSPlayerController))
	{
		DSPlayerController->PlayerSessionId = PlayerSessionId;
		DSPlayerController->Username = Username;
	}
	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::InitNewPlayer - PlayerSessionId: %s, Username: %s"), *PlayerSessionId, *Username);

	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
	{
		AddPlayerInfoToLobbyState(NewPlayerController);
	}
	return InitializedString;
}

void ADSLobbyGameMode::CancelCountdown()
{
	if (GetNumPlayers() - 1 < MinPlayers && LobbyStatus == ELobbyStatus::CountdownToSeamlessTravel)
	{
		LobbyStatus = ELobbyStatus::WaitingForPlayers;
		StopCountdownTimer(LobbyCountdownTimer);
	}
}

void ADSLobbyGameMode::InitGameLift()
{
	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		if (DSGameInstanceSubsystem = GameInstance->GetSubsystem<UDSGameInstanceSubsystem>(); IsValid(DSGameInstanceSubsystem))
		{
			FServerParameters ServerParameters;
			SetServerParameters(ServerParameters);
			DSGameInstanceSubsystem->InitGameLift(ServerParameters);
		}
	}
}

void ADSLobbyGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	//The Host/compute-name of the GameLift Anywhere instance.
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}

	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}

	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}

	//The PID of the running process
	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
	UE_LOG(LogDedicatedServers, Log, TEXT("PID: %s"), *OutServerParameters.m_processId);
}

void ADSLobbyGameMode::AddPlayerInfoToLobbyState(AController* Player) const
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Player);
	ADSGameState* DSGameState = GetGameState<ADSGameState>();
	if (IsValid(DSGameState) && IsValid(DSGameState->LobbyState) && IsValid(DSPlayerController))
	{
		FLobbyPlayerInfo PlayerInfo(DSPlayerController->Username);
		DSGameState->LobbyState->AddPlayerInfo(PlayerInfo);
	}
}
void ADSLobbyGameMode::RemovePlayerInfoFromLobbyState(AController* Player) const
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Player);
	ADSGameState* DSGameState = GetGameState<ADSGameState>();
	if (IsValid(DSGameState) && IsValid(DSGameState->LobbyState) && IsValid(DSPlayerController))
	{
		DSGameState->LobbyState->RemovePlayerInfo(DSPlayerController->Username);
	}
}

void ADSLobbyGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage)
{

	if (PlayerSessionId.IsEmpty() || Username.IsEmpty())
	{
		OutErrorMessage = TEXT("PlayerSessionId and/or Username invalid.");
		return;
	}

#if WITH_GAMELIFT
	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest DescribePlayerSessionsRequest;
	DescribePlayerSessionsRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));

	const auto& DescribePlayerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(DescribePlayerSessionsRequest);
	if (!DescribePlayerSessionsOutcome.IsSuccess())
	{
		OutErrorMessage = TEXT("DescribePlayerSessions failed.");
		return;
	}

	const auto& DescribePlayerSessionsResult = DescribePlayerSessionsOutcome.GetResult();
	int32 Count = 0;
	const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = DescribePlayerSessionsResult.GetPlayerSessions(Count);
	if (PlayerSessions == nullptr || Count == 0)
	{
		OutErrorMessage = TEXT("GetPlayerSessions failed.");
		return;
	}

	for (int32 i = 0; i < Count; i++)
	{
		const Aws::GameLift::Server::Model::PlayerSession& PlayerSession = PlayerSessions[i];
		if (!Username.Equals(PlayerSession.GetPlayerId())) continue;
		if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
		{
			OutErrorMessage = FString::Printf(TEXT("Session for %s not RESERVED; Fail PreLogin."), *Username);
			return;
		}

		const auto& AcceptPlayerSessionOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
		OutErrorMessage = AcceptPlayerSessionOutcome.IsSuccess() ? "" : FString::Printf(TEXT("Failed to accept player session for %s"), *Username);
	}
#endif
}
