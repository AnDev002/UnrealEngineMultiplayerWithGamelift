// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameMode.h"

DEFINE_LOG_CATEGORY(LogDS_GameMode);
void ADS_GameMode::BeginPlay()
{
	Super::BeginPlay();

#if WITH_GAMELIFT
	InitGameLift();
#endif 
}

void ADS_GameMode::InitGameLift()
{
	UE_LOG(LogDS_GameMode, Log, TEXT("Initializing the GameLift Server"));

	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	//Define the server parameters for an Amazon GameLift Servers Anywhere fleet. These are not needed for an Amazon GameLift Servers managed EC2 fleet.
	FServerParameters ServerParameters;

	SetServerParameters(ServerParameters);

	//InitSDK establishes a local connection with the Amazon GameLift Servers Agent to enable further communication.
	//Use InitSDK(serverParameters) for an Amazon GameLift Servers Anywhere fleet. 
	//Use InitSDK() for Amazon GameLift Servers managed EC2 fleet.
	GameLiftSdkModule->InitSDK(ServerParameters);

	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
		{
			FString GameSessionId = FString(gameSession.GetGameSessionId());
			UE_LOG(LogDS_GameMode, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
			GameLiftSdkModule->ActivateGameSession();
		};

	ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

	auto OnProcessTerminate = [=]()
		{
			UE_LOG(LogDS_GameMode, Log, TEXT("Game Server Process Is Terminating"));
			GameLiftSdkModule->ProcessEnding();
		};

	ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

	//Implement callback function OnHealthCheck
	//Amazon GameLift Servers invokes this callback approximately every 60 seconds.
	//A game server might want to check the health of dependencies, etc.
	//Then it returns health status true if healthy, false otherwise.
	//The game server must respond within 60 seconds, or Amazon GameLift Servers records 'false'.
	//In this example, the game server always reports healthy.
	auto OnHealthCheck = []()
		{
			UE_LOG(LogDS_GameMode, Log, TEXT("Performing Health Check"));
			return true;
		};

	ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

	int32 Port = FURL::UrlConfig.DefaultPort;

	ParseCommandLinePort(Port);

	ProcessParameters.port = Port;

	TArray<FString> LogFiles;
	LogFiles.Add(TEXT("TCOS_Multiplayer/Saved/Logs/TCOS_Multiplayer.log"));
	ProcessParameters.logParameters = LogFiles;

	UE_LOG(LogDS_GameMode, Log, TEXT("Calling Process Ready!"));
	GameLiftSdkModule->ProcessReady(ProcessParameters);
}

void ADS_GameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("-awsregion="), OutServerParameters.m_awsRegion))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("AWS_REGION: %s"), *OutServerParameters.m_awsRegion)
	}


	if (FParse::Value(FCommandLine::Get(), TEXT("-accesskey="), OutServerParameters.m_accessKey))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("ACCESS_KEY: %s"), *OutServerParameters.m_accessKey)
	}
	if (FParse::Value(FCommandLine::Get(), TEXT("-secretkey="), OutServerParameters.m_secretKey))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("SECRET_KEY: % s"), *OutServerParameters.m_secretKey)
	}
	if (FParse::Value(FCommandLine::Get(), TEXT("-sessiontoken="), OutServerParameters.m_sessionToken))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("SESSION_TOKEN: %s"), *OutServerParameters.m_sessionToken)
	}

	//The Host/compute-name of the Amazon GameLift Servers Anywhere instance.
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}

	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}

	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}


	FString glProcessId = "";
	if (FParse::Value(FCommandLine::Get(), TEXT("-processId="), glProcessId))
	{
		OutServerParameters.m_processId = TCHAR_TO_UTF8(*glProcessId);
	}
	else
	{
		// If no ProcessId is passed as a command line argument, generate a randomized unique string.
		FString TimeString = FString::FromInt(std::time(nullptr));
		FString ProcessId = "ProcessId_" + TimeString;
		OutServerParameters.m_processId = TCHAR_TO_UTF8(*ProcessId);
	}

	//The PID of the running process
	UE_LOG(LogDS_GameMode, Log, TEXT("PID: %s"), *OutServerParameters.m_processId);
}

void ADS_GameMode::ParseCommandLinePort(int32& OutPort)
{
	TArray<FString> CommandLineTokens;
	TArray<FString> CommandLineSwitches;
	FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);

	for (const FString& Switch : CommandLineSwitches)
	{
		FString Key;
		FString Value;
		if (Switch.Split("=", &Key, &Value))
		{
			if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase))
			{
				OutPort = FCString::Atoi(*Value);
				return;
			}
		}
	}
}
