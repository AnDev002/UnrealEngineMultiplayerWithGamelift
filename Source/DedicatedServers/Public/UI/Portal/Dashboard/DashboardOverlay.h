// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardOverlay.generated.h"

class UWidgetSwitcher;
class UGamePage;
class UCareerPage;
class ULeaderboardPage;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDashboardOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGamePage> Page_Game;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCareerPage> Page_Career;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULeaderboardPage> Page_Leaderboard;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Game;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Career;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Leaderboard;

private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ShowGamePage();

	UFUNCTION()
	void ShowCareerPage();

	UFUNCTION()
	void ShowLeaderboardPage();
};
