// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePage.generated.h"

class UJoinGame;
class UGameSessionsManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UGamePage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UJoinGame> JoinGameWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameSessionsManager> GameSessionsManagerClass;

protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void JoinGameButtonClicked();

	UPROPERTY()
	TObjectPtr<UGameSessionsManager> GameSessionsManager;
};
