#include "DataTypes/HTTPRequestTypes/HTTPRequestTypes.h"

//하드 코딩 방지 namespace활용
namespace HTTPStatusMessages
{
	const FString SomethingWentWrong = TEXT("Something went wrong");
}


void FDSMetaData::Dump() const
{
	UE_LOG(LogTemp, Log, TEXT("FDSMetaData Dump:"));
	UE_LOG(LogTemp, Log, TEXT("HttpStatusCode: %d"), httpStatusCode);
	UE_LOG(LogTemp, Log, TEXT("RequestId: %s"), *requestId);
	UE_LOG(LogTemp, Log, TEXT("Attempts: %d"), attempts);
	UE_LOG(LogTemp, Log, TEXT("TotalRetryDelay: %f"), totalRetryDelay);
}


void FDSListFleetsResponse::Dump() const
{
	// Print NextToken
	UE_LOG(LogTemp, Log, TEXT("FDSListFleetsResponse::Dump() - NextToken: %s"), *NextToken);

	// Print all FleetIds
	UE_LOG(LogTemp, Log, TEXT("FleetIds:"));
	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(LogTemp, Log, TEXT(" - %s"), *FleetId);
	}

	// If there's nothing in the array
	if (FleetIds.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT(" - No FleetIds available"));
	}
}

void FDSGameSession::Dump() const
{
    UE_LOG(LogTemp, Log, TEXT("FDSGameSession Dump:"));
    UE_LOG(LogTemp, Log, TEXT("CreationTime: %s"), *CreationTime);
    UE_LOG(LogTemp, Log, TEXT("CreatorId: %s"), *CreatorId);
    UE_LOG(LogTemp, Log, TEXT("CurrentPlayerSessionCount: %d"), CurrentPlayerSessionCount);
    UE_LOG(LogTemp, Log, TEXT("DnsName: %s"), *DnsName);
    UE_LOG(LogTemp, Log, TEXT("FleetArn: %s"), *FleetArn);
    UE_LOG(LogTemp, Log, TEXT("FleetId: %s"), *FleetId);

    UE_LOG(LogTemp, Log, TEXT("GameProperties:"));
    for (const auto& Property : GameProperties)
    {
        UE_LOG(LogTemp, Log, TEXT("  Key: %s, Value: %s"), *Property.Key, *Property.Value);
    }

    UE_LOG(LogTemp, Log, TEXT("GameSessionData: %s"), *GameSessionData);
    UE_LOG(LogTemp, Log, TEXT("GameSessionId: %s"), *GameSessionId);
    UE_LOG(LogTemp, Log, TEXT("IpAddress: %s"), *IpAddress);
    UE_LOG(LogTemp, Log, TEXT("Location: %s"), *Location);
    UE_LOG(LogTemp, Log, TEXT("MatchmakerData: %s"), *MatchmakerData);
    UE_LOG(LogTemp, Log, TEXT("MaximumPlayerSessionCount: %d"), MaximumPlayerSessionCount);
    UE_LOG(LogTemp, Log, TEXT("Name: %s"), *Name);
    UE_LOG(LogTemp, Log, TEXT("PlayerSessionCreationPolicy: %s"), *PlayerSessionCreationPolicy);
    UE_LOG(LogTemp, Log, TEXT("Port: %d"), Port);
    UE_LOG(LogTemp, Log, TEXT("Status: %s"), *Status);
    UE_LOG(LogTemp, Log, TEXT("StatusReason: %s"), *StatusReason);
    UE_LOG(LogTemp, Log, TEXT("TerminationTime: %s"), *TerminationTime);
}

void FDSPlayerSession::Dump() const
{
	UE_LOG(LogTemp, Log, TEXT("PlayerSession:"));
	UE_LOG(LogTemp, Log, TEXT("  CreationTime: %s"), *CreationTime);
	UE_LOG(LogTemp, Log, TEXT("  DnsName: %s"), *DnsName);
	UE_LOG(LogTemp, Log, TEXT("  FleetArn: %s"), *FleetArn);
	UE_LOG(LogTemp, Log, TEXT("  FleetId: %s"), *FleetId);
	UE_LOG(LogTemp, Log, TEXT("  GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogTemp, Log, TEXT("  IpAddress: %s"), *IpAddress);
	UE_LOG(LogTemp, Log, TEXT("  PlayerData: %s"), *PlayerData);
	UE_LOG(LogTemp, Log, TEXT("  PlayerId: %s"), *PlayerId);
	UE_LOG(LogTemp, Log, TEXT("  PlayerSessionId: %s"), *PlayerSessionId);
	UE_LOG(LogTemp, Log, TEXT("  Port: %d"), Port);
	UE_LOG(LogTemp, Log, TEXT("  Status: %s"), *Status);
	UE_LOG(LogTemp, Log, TEXT("  TerminationTime: %s"), *TerminationTime);
}


void FDSCodeDeliveryDetails::Dump() const
{
	UE_LOG(LogTemp, Log, TEXT("CodeDeliveryDetails:"));
	UE_LOG(LogTemp, Log, TEXT("   AttributeName: %s"), *AttributeName);
	UE_LOG(LogTemp, Log, TEXT("   DeliveryMedium: %s"), *DeliveryMedium);
	UE_LOG(LogTemp, Log, TEXT("   Destination: %s"), *Destination);
}

void FDSSignUpResponse::Dump() const
{
	UE_LOG(LogTemp, Log, TEXT("FDSSignUpResponse Details:"));
	CodeDeliveryDetails.Dump();
	UE_LOG(LogTemp, Log, TEXT("UserConfirmed: %s"), UserConfirmed ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Log, TEXT("UserSub: %s"), *UserSub);
}

void FDSNewDeviceMetaData::Dump() const
{
	UE_LOG(LogTemp, Log, TEXT("NewDeviceMetadata:"));
	UE_LOG(LogTemp, Log, TEXT("   DeviceGroupKey: %s"), *DeviceGroupKey);
	UE_LOG(LogTemp, Log, TEXT("   DeviceKey: %s"), *DeviceKey);
}

void FDSAuthenticationResult::Dump() const
{
	UE_LOG(LogTemp, Log, TEXT("AuthenticationResult:"));
	UE_LOG(LogTemp, Log, TEXT("   AccessToken: %s"), *AccessToken);
	UE_LOG(LogTemp, Log, TEXT("   ExpiresIn: %d"), ExpiresIn);
	UE_LOG(LogTemp, Log, TEXT("   IdToken: %s"), *IdToken);
	NewDeviceMetadata.Dump();
	UE_LOG(LogTemp, Log, TEXT("   RefreshToken: %s"), *RefreshToken);
	UE_LOG(LogTemp, Log, TEXT("   TokenType: %s"), *TokenType);
}

void FDSChallengeParameters::Dump() const
{
	UE_LOG(LogTemp, Log, TEXT("ChallengeParameters:"));
	for (const auto& Param : Parameters)
	{
		UE_LOG(LogTemp, Log, TEXT("   %s: %s"), *Param.Key, *Param.Value);
	}
}

void FDSInitiateAuthResponse::Dump() const
{
	UE_LOG(LogTemp, Log, TEXT("FDSSignUpResponse Details:"));
	AuthenticationResult.Dump();
	UE_LOG(LogTemp, Log, TEXT("ChallengeName: %s"), *ChallengeName);
	ChallengeParameters.Dump();
	UE_LOG(LogTemp, Log, TEXT("Session: %s"), *Session);
	UE_LOG(LogTemp, Log, TEXT("email: %s"), *email);
}












