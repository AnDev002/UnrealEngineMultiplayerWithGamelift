// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignUpPage.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USignUpPage::UpdateStatusMessage(const FString& Message, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
	if (bShouldResetWidgets)
	{
		Button_SignUp->SetIsEnabled(true);
	}
}

void USignUpPage::ClearTextBoxes()
{
	TextBox_UserName->SetText(FText::GetEmpty());
	TextBox_Password->SetText(FText::GetEmpty());
	TextBox_ConfirmPassword->SetText(FText::GetEmpty());
	TextBox_Email->SetText(FText::GetEmpty());
}

void USignUpPage::NativeConstruct()
{
	Super::NativeConstruct();

	TextBox_UserName->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_Password->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_ConfirmPassword->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_Email->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	Button_SignUp->SetIsEnabled(false);
}

void USignUpPage::UpdateSignUpButtonState(const FText& Text)
{
	const bool bIsUsernameValid = !TextBox_UserName->GetText().ToString().IsEmpty();
	const bool bArePasswordsEqual = TextBox_Password->GetText().ToString() == TextBox_ConfirmPassword->GetText().ToString();
	const bool bIsValidEmail = IsValidEmail(TextBox_Email->GetText().ToString());
	const bool bPasswordLongEnought = TextBox_Password->GetText().ToString().Len() >= 8;

	if (!bPasswordLongEnought)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Password must contain at least 8 character.")));
	}
	else if(!bIsUsernameValid)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please enter a valid Username.")));
	}
	else if (!bArePasswordsEqual)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please ensure that password match.")));
	}
	else if (!bIsValidEmail)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please ensure email match.")));
	}
	else
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("")));
		Button_SignUp->SetIsEnabled(true);
	}
	ECheckPassword checkPassword = IsStrongPassword(TextBox_Password->GetText().ToString());
	switch (checkPassword)
	{
	case ECheckPassword::NONE:
		break;
	case ECheckPassword::SPECIAL_CHARACTER:
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Password must contain at least one special character.")));
		break;
	case ECheckPassword::NUMBER:
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Password must contain at least one number.")));
		break;
	case ECheckPassword::UPPERCASE:
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Password must contain at least one uppercase character.")));
		break;
	case ECheckPassword::LOWERCASE:
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Password must contain at least one lowercase character.")));
		break;
	default:
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("")));
		Button_SignUp->SetIsEnabled(true);
		break;
	}
}

bool USignUpPage::IsValidEmail(const FString& Email)
{
	const FRegexPattern EmailPattern(TEXT(R"((^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$))"));
	FRegexMatcher Matcher(EmailPattern, Email);
	return Matcher.FindNext();
}

ECheckPassword USignUpPage::IsStrongPassword(const FString& Password)
{
	const FRegexPattern NumberPattern(TEXT(R"(\d)")); // contain at least one number
	const FRegexPattern SpecialCharPattern(TEXT(R"([^\w\s])")); // contain at least one special character
	const FRegexPattern UppercasePattern(TEXT(R"([A-Z])")); // contain at least one uppercase character
	const FRegexPattern LowercasePattern(TEXT(R"([a-z])")); // contain at least one lowercase character
	
	FRegexMatcher NumberMatcher(NumberPattern, Password);
	FRegexMatcher SpecialCharMatcher(SpecialCharPattern, Password);
	FRegexMatcher UppercaseMatcher(UppercasePattern, Password);
	FRegexMatcher LowercaseMatcher(LowercasePattern, Password);

	if (!NumberMatcher.FindNext())
	{
		return ECheckPassword::NUMBER;
	}
	if (!SpecialCharMatcher.FindNext())
	{
		return ECheckPassword::SPECIAL_CHARACTER;
	}
	if (!UppercaseMatcher.FindNext())
	{
		return ECheckPassword::UPPERCASE;
	}
	if (!LowercaseMatcher.FindNext())
	{
		return ECheckPassword::LOWERCASE;
	}

	return ECheckPassword::NONE;
}
