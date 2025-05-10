// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/DashboardOverlay.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Portal/Dashboard/GamePage.h"
#include "UI/Portal/Dashboard/CareerPage.h"
#include "UI/Portal/Dashboard/LeaderboardPage.h"
#include "Components/Button.h"

void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Game->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowGamePage);
	Button_Career->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCareerPage);
	Button_Leaderboard->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowLeaderboardPage);

	ShowGamePage();
}

void UDashboardOverlay::ShowGamePage()
{
	WidgetSwitcher->SetActiveWidget(Page_Game);
}

void UDashboardOverlay::ShowCareerPage()
{
	WidgetSwitcher->SetActiveWidget(Page_Career);
}

void UDashboardOverlay::ShowLeaderboardPage()
{
	WidgetSwitcher->SetActiveWidget(Page_Leaderboard);

}
