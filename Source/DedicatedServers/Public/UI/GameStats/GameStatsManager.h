// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "GameStatsManager.generated.h"

struct FDSRecordMatchStatsInput;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UGameStatsManager : public UHTTPRequestManager
{
	GENERATED_BODY()
	
	

public:
	void RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput);

private:

	void RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
