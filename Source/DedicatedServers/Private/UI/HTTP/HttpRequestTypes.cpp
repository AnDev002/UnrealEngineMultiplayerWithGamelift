#include "UI/HTTP/HttpRequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

namespace HTTPStatusMessages
{
	const FString SomethingWentWrong{TEXT("Something went wrong!")};
}

void FDSMetaData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("MetaData:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("httpStatusCode: %d"), httpStatusCode);
	UE_LOG(LogDedicatedServers, Log, TEXT("requestId: %s"), *requestId);
	UE_LOG(LogDedicatedServers, Log, TEXT("attempts: %d"), attempts);
	UE_LOG(LogDedicatedServers, Log, TEXT("totalRetryDelay: %f"), totalRetryDelay);
}

void FDSListFleetsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("ListFleetsResponse:"));
	for (const FString FleetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
	}
	if (NextToken.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("NextToken: %s"), *NextToken);
	}
}

void FDSGameSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSession:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationID: %s"), *CreationId);
	UE_LOG(LogDedicatedServers, Log, TEXT("CurrentPlayerSessionCount: %d"), CurrentPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameProperties: "));
	for (const auto& GameProperty : GameProperties)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("%s: %s"), *GameProperty.Key, *GameProperty.Value);
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionData: %s"), *GameSessionData);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("Location: %s"), *Location);
	UE_LOG(LogDedicatedServers, Log, TEXT("MatchmakerData: %s"), *MatchmakerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("MaximumPlayerSessionCount: %d"), MaximumPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("Name: %s"), *Name);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSessionCreationPolicy: %s"), *PlayerSessionCreationPolicy);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("StatusReason: %s"), *StatusReason);
	UE_LOG(LogDedicatedServers, Log, TEXT("TerminationTime: %s"), *TerminationTime);
}

void FDSPlayerSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSession:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerData: %s"), *PlayerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerId: %s"), *PlayerId);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSessionId: %s"), *PlayerSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("TerminationTime: %s"), *TerminationTime);
}

void FCodeDeliveryDetails::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Code Delivery Details:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("AttributeName: %s"), *AttributeName);
	UE_LOG(LogDedicatedServers, Log, TEXT("DeliveryMedium: %s"), *DeliveryMedium);
	UE_LOG(LogDedicatedServers, Log, TEXT("Destination: %s"), *Destination);
}

void FDSSignUpResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Sign Up Response:"));
	CodeDeliveryDetails.Dump();
	if (UserConfirmed)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: true"));
	}
	else
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: false"));
	}
	UE_LOG(LogDedicatedServers, Log, TEXT("UserSub: %s"), *UserSub);
}

void FNewDeviceMetadata::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("New Device Metadata:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("DeviceGroupKey: %s"), *DeviceGroupKey);
	UE_LOG(LogDedicatedServers, Log, TEXT("DeviceKey: %s"), *DeviceKey);
}

void FAuthenticationResult::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Authentication Result:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("AccessToken: %s"), *AccessToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("ExpiresIn: %d"), ExpiresIn);
	UE_LOG(LogDedicatedServers, Log, TEXT("IdToken: %s"), *IdToken);
	NewDeviceMetadata.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("RefreshToken: %s"), *RefreshToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("TokenType: %s"), *TokenType);

}

void FChallengeParameters::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Challenge Parameters:"));
	for (const auto& Param : Parameters)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("		%s: %s"), *Param.Key, *Param.Value);

	}
}

void FDSInitiateAuthResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Initiate Auth Response:"));
	AuthenticationResult.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("ChallengeName: %s"), *ChallengeName);
	ChallengeParameters.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %s"), *Session);
	UE_LOG(LogDedicatedServers, Log, TEXT("Email: %s"), *Email);
}
