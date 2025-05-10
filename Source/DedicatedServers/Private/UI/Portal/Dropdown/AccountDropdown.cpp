// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dropdown/AccountDropdown.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "Components/TextBlock.h"

void UAccountDropdown::NativeConstruct()
{
	Super::NativeConstruct();

	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		TextBlock_ButtonText->SetText(FText::FromString(LocalPlayerSubsystem->Username));
	}
}

UDSLocalPlayerSubsystem* UAccountDropdown::GetLocalPlayerSubsystem() const
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
