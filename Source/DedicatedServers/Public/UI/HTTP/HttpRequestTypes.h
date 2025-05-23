#pragma once

#include "HttpRequestTypes.generated.h"

namespace HTTPStatusMessages
{
	extern DEDICATEDSERVERS_API const FString SomethingWentWrong;
}

USTRUCT()
struct FDSMetaData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 httpStatusCode{};

	UPROPERTY()
	FString requestId{};

	UPROPERTY()
	int32 attempts{};

	UPROPERTY()
	double totalRetryDelay{};

	void Dump() const;
};

USTRUCT()
struct FDSListFleetsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> FleetIds{};

	UPROPERTY()
	FString NextToken{};

	void Dump() const;
};

USTRUCT()
struct FDSGameSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString CreationTime{};
	
	UPROPERTY()
	FString CreationId{};
	
	UPROPERTY()
	int32 CurrentPlayerSessionCount{};
	
	UPROPERTY()
	FString DnsName{};
	
	UPROPERTY()
	FString FleetArn{};
	
	UPROPERTY()
	FString FleetId{};
	
	UPROPERTY()
	TMap<FString, FString> GameProperties{};
	
	UPROPERTY()
	FString GameSessionData{};
	
	UPROPERTY()
	FString GameSessionId{};
	
	UPROPERTY()
	FString IpAddress{};
	
	UPROPERTY()
	FString Location{};
	
	UPROPERTY()
	FString MatchmakerData{};

	UPROPERTY()
	int32 MaximumPlayerSessionCount{};

	UPROPERTY()
	FString Name{};

	UPROPERTY()
	FString PlayerSessionCreationPolicy{};

	UPROPERTY()
	int32 Port{};

	UPROPERTY()
	FString Status{};

	UPROPERTY()
	FString StatusReason{};

	UPROPERTY()
	FString TerminationTime{};

	void Dump() const;
};

USTRUCT()
struct FDSPlayerSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString CreationTime{};

	UPROPERTY()
	FString DnsName{};

	UPROPERTY()
	FString FleetArn{};

	UPROPERTY()
	FString FleetId{};

	UPROPERTY()
	FString GameSessionId{};

	UPROPERTY()
	FString IpAddress{};

	UPROPERTY()
	FString PlayerData{};

	UPROPERTY()
	FString PlayerId{};

	UPROPERTY()
	FString PlayerSessionId{};

	UPROPERTY()
	int32 Port{};

	UPROPERTY()
	FString Status{};

	UPROPERTY()
	FString TerminationTime{};

	void Dump() const;
};

USTRUCT()
struct FCodeDeliveryDetails
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString AttributeName{};
	
	UPROPERTY()
	FString DeliveryMedium{};
	
	UPROPERTY()
	FString Destination{};

	void Dump() const;
};

USTRUCT(BlueprintType)
struct FDSSignUpResponse
{
	GENERATED_BODY()
	
	UPROPERTY()
	FCodeDeliveryDetails CodeDeliveryDetails{};
	
	UPROPERTY()
	bool UserConfirmed{};
	
	UPROPERTY()
	FString UserSub{};

	void Dump() const;
};


USTRUCT(BlueprintType)
struct FNewDeviceMetadata
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString DeviceGroupKey{};
	
	UPROPERTY()
	FString DeviceKey{};

	void Dump() const;
};

USTRUCT(BlueprintType)
struct FAuthenticationResult
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString AccessToken{};
	
	UPROPERTY()
	int32 ExpiresIn{};

	UPROPERTY()
	FString IdToken{};

	UPROPERTY()
	FNewDeviceMetadata NewDeviceMetadata{};

	UPROPERTY()
	FString RefreshToken{};

	UPROPERTY()
	FString TokenType{};

	void Dump() const;
};

USTRUCT(BlueprintType)
struct FChallengeParameters
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<FString, FString> Parameters{};

	void Dump() const;
};


USTRUCT(BlueprintType)
struct FDSInitiateAuthResponse
{
	GENERATED_BODY()
	
	UPROPERTY()
	FAuthenticationResult AuthenticationResult{};
	
	UPROPERTY()
	FString ChallengeName{};

	UPROPERTY()
	FChallengeParameters ChallengeParameters{};

	UPROPERTY()
	FString Session{};

	UPROPERTY()
	FString Email{};

	void Dump() const;
};
USTRUCT()
struct FDSMatchStats
{
	GENERATED_BODY()

	UPROPERTY()
	int32 score{};

	UPROPERTY()
	int32 defeats{};

	UPROPERTY()
	int32 hits{};

	UPROPERTY()
	int32 highestStreak{};

	UPROPERTY()
	int32 gotFirstBlood{};

	UPROPERTY()
	int32 matchWins{};

	UPROPERTY()
	int32 matchLosses{};
};

USTRUCT()
struct FDSRecordMatchStatsInput
{
	GENERATED_BODY()

	UPROPERTY()
	FDSMatchStats matchStats{};

	UPROPERTY()
	FString username{};
};

