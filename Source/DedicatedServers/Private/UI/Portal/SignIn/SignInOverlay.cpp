// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInOverlay.h"
#include "UI/API/GameSessions/JoinGame.h"
#include "Components/Button.h"
#include "UI/Portal/PortalManager.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Portal/SignIn/SignInPage.h"
#include "UI/Portal/SignIn/SignUpPage.h"
#include "UI/Portal/SignIn/SuccessConfirmedPage.h"
#include "UI/Portal/SignIn/ConfirmSignUpPage.h"
#include "Components/EditableTextBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/GameSessions/GameSessionsManager.h"
#include "Components/TextBlock.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(PortalManagerClass);
	check(GameSessionsManagerClass);

	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	GameSessionsManager = NewObject<UGameSessionsManager>(this, GameSessionsManagerClass);


	Page_SignIn->Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::SignInButtonClicked);
	Page_SignIn->Button_Quit->OnClicked.AddDynamic(this, &USignInOverlay::QuitButtonClicked);
	Page_SignIn->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	PortalManager->SignInStatusMessageDelegate.AddDynamic(Page_SignIn, &USignInPage::UpdateStatusMessage);

	Page_SignUp->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::SignUpButtonClicked);
	Page_SignUp->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	PortalManager->SignUpStatusMessageDelegate.AddDynamic(Page_SignUp, &USignUpPage::UpdateStatusMessage);
	PortalManager->OnSignUpSucceeded.AddDynamic(this, &USignInOverlay::OnSignUpSucceeded);

	Page_ConfirmSignUp->Button_Confirm->OnClicked.AddDynamic(this, &USignInOverlay::ConfirmSignUpButtonClicked);
	Page_ConfirmSignUp->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	PortalManager->OnConfirmSucceeded.AddDynamic(this, &USignInOverlay::OnConfirmSucceeded);
	PortalManager->ConfirmStatusMessageDelegate.AddDynamic(Page_ConfirmSignUp, &UConfirmSignUpPage::UpdateStatusMessage);

	Page_SuccessConfirmed->Button_Ok->OnClicked.AddDynamic(this, &USignInOverlay::OkButtonClicked);

}

void USignInOverlay::ShowSignInPage()
{
	WidgetSwitcher->SetActiveWidget(Page_SignIn);
}

void USignInOverlay::ShowSignUpPage()
{
	WidgetSwitcher->SetActiveWidget(Page_SignUp);

}

void USignInOverlay::ShowConfirmSignUpPage()
{
	WidgetSwitcher->SetActiveWidget(Page_ConfirmSignUp);

}

void USignInOverlay::ShowSuccessConfirmedPage()
{
	WidgetSwitcher->SetActiveWidget(Page_SuccessConfirmed);
}

void USignInOverlay::SignInButtonClicked()
{
	const FString UserName = Page_SignIn->TextBox_UserName->GetText().ToString();
	const FString Password = Page_SignIn->TextBox_Password->GetText().ToString();
	PortalManager->SignIn(UserName, Password);
}

void USignInOverlay::SignUpButtonClicked()
{
	const FString UserName = Page_SignUp->TextBox_UserName->GetText().ToString();
	const FString Password = Page_SignUp->TextBox_Password->GetText().ToString();
	const FString ConfirmationPassword = Page_SignUp->TextBox_ConfirmPassword->GetText().ToString();
	const FString Email = Page_SignUp->TextBox_Email->GetText().ToString();
	PortalManager->SignUp(UserName, Password, Email);
}

void USignInOverlay::QuitButtonClicked()
{
	PortalManager->QuitGame();
}

void USignInOverlay::ConfirmSignUpButtonClicked()
{
	const FString ConfirmationCode = Page_ConfirmSignUp->TextBox_ConfirmationCode->GetText().ToString();
	Page_ConfirmSignUp->Button_Confirm->SetIsEnabled(false);
	PortalManager->ConfirmSignUp(ConfirmationCode);
}

void USignInOverlay::OkButtonClicked()
{
	ShowSignInPage();
}

void USignInOverlay::OnSignUpSucceeded()
{
	Page_SignUp->ClearTextBoxes();
	Page_ConfirmSignUp->TextBlock_Destination->SetText(FText::FromString(PortalManager->LastSignUpResponse.CodeDeliveryDetails.Destination));
	ShowConfirmSignUpPage();
}
void USignInOverlay::OnConfirmSucceeded()
{
	Page_ConfirmSignUp->ClearTextBoxes();
	ShowSuccessConfirmedPage();
}
//
//UPROPERTY(meta = (BindWidget))
//TObjectPtr<UJoinGame> JoinGameWidget;
//
//UFUNCTION()
//void OnJoinGameButtonClicked();
//UFUNCTION()
//void UpdataJoinGameStatusMessage(const FString& StatusMessage, bool bResetJoinGameButton);
//JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &USignInOverlay::OnJoinGameButtonClicked);
//void USignInOverlay::OnJoinGameButtonClicked()
//{
//	GameSessionsManager->BroadcastJoinGameSessionMessage.AddDynamic(this, &USignInOverlay::UpdataJoinGameStatusMessage);
//	GameSessionsManager->JoinGameSession();
//	JoinGameWidget->Button_JoinGame->SetIsEnabled(false);
//}
//
//void USignInOverlay::UpdataJoinGameStatusMessage(const FString& StatusMessage, bool bResetJoinGameButton)
//{
//	JoinGameWidget->SetStatusMessage(StatusMessage);
//
//	if (bResetJoinGameButton)
//	{
//		JoinGameWidget->Button_JoinGame->SetIsEnabled(true);
//	}
//}