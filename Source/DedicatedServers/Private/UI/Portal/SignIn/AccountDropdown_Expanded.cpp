// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/AccountDropdown_Expanded.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/Portal/PortalManager.h"
#include "Player/DSLocalPlayerSubsystem.h"
void UAccountDropdown_Expanded::NativeConstruct()
{
	Super::NativeConstruct();
	//check(PortalManagerClass);
	//PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	Button_SignOut->OnClicked.AddDynamic(this, &UAccountDropdown_Expanded::SignOutButton_OnClicked);
	Button_SignOut->OnHovered.AddDynamic(this, &UAccountDropdown_Expanded::SignOutButtonHover);
	Button_SignOut->OnUnhovered.AddDynamic(this, &UAccountDropdown_Expanded::SignOutButtonUnhover);

	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		TextBlock_Email->SetText(FText::FromString(LocalPlayerSubsystem->Email));
	}
}

void UAccountDropdown_Expanded::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetSignOutButtonStyleTransparent();
	SignOutButtonUnhover();
}

void UAccountDropdown_Expanded::SetSignOutButtonStyleTransparent()
{
	FButtonStyle Style;
	FSlateBrush Brush;
	Brush.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
	Style.Disabled = Brush;
	Style.Hovered = Brush;
	Style.Pressed = Brush;
	Style.Normal = Brush;
	Button_SignOut->SetStyle(Style);
}

UDSLocalPlayerSubsystem* UAccountDropdown_Expanded::GetLocalPlayerSubsystem()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) && IsValid(PlayerController->GetLocalPlayer()))
	{
		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UDSLocalPlayerSubsystem>();
		if (IsValid(LocalPlayerSubsystem))
		{
			return LocalPlayerSubsystem;
		}
	}
	return nullptr;
}

void UAccountDropdown_Expanded::SignOutButtonHover()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(HoveredTextColor);
}

void UAccountDropdown_Expanded::SignOutButtonUnhover()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(UnhoveredTextColor);
}

void UAccountDropdown_Expanded::SignOutButton_OnClicked()
{
	Button_SignOut->SetIsEnabled(false);

	check(PortalManager)
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		FAuthenticationResult AuthResult = LocalPlayerSubsystem->GetAuthResult();
		PortalManager->SignOut(AuthResult.AccessToken);
	}
}
