// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInPage.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
void USignInPage::UpdateStatusMessage(const FString& Message, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
	if (bShouldResetWidgets)
	{
		Button_SignIn->SetIsEnabled(true);
	}
}
