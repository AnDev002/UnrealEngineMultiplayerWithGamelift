// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/API/APIData.h"

FString UAPIData::GetAPIEndpoint(const FGameplayTag& APIEndPoint)
{
	const FString ResourceName = Resources.FindChecked(APIEndPoint);
	return InvokeURL + "/" + Stage + "/" + ResourceName;
}
