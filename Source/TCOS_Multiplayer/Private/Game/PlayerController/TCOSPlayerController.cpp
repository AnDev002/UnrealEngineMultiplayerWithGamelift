

#include "Game/PlayerController/TCOSPlayerController.h"
#include "Inventory/InventoryComponent.h"
#include "UI/WidgetControllers/InventoryWidgetController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UI/TCOSSystemsWidget.h"
#include "Input/TCOSSystemInputComponent.h"
#include "AbilitySystem/TCOSAbilitySystemComponent.h"
#include "Game/PlayerState/TCOSPlayerState.h"
#include "Input/TCOSInputConfig.h"
#include "Interfaces/PlayerInterface.h"
#include "InputAction.h"
#include "Net/UnrealNetwork.h"

ATCOSPlayerController::ATCOSPlayerController()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InventoryComponent->SetIsReplicated(true);
}

void ATCOSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UTCOSSystemInputComponent* TCOSInputComp = Cast<UTCOSSystemInputComponent>(InputComponent))
	{
		TCOSInputComp->BindAbilityActions(TCOSInputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
	}
}

UInventoryComponent* ATCOSPlayerController::GetInventoryComponent_Implementation()
{
	return InventoryComponent;
}

UAbilitySystemComponent* ATCOSPlayerController::GetAbilitySystemComponent() const
{
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
}

UInventoryWidgetController* ATCOSPlayerController::GetInventoryWidgetController()
{
	if (!IsValid(InventoryWidgetController))
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetOwningActor(this);
		InventoryWidgetController->BindCallbacksToDependencies();
	}
	return InventoryWidgetController;
}

void ATCOSPlayerController::CreateInventoryWidget()
{
	if (UUserWidget* Widget = CreateWidget<UTCOSSystemsWidget>(this, InventoryWidgetClass))
	{
		InventoryWidget = Cast<UTCOSSystemsWidget>(Widget);
		InventoryWidget->SetWidgetController(GetInventoryWidgetController());
		InventoryWidgetController->BroadcastInitialValues();
		InventoryWidget->AddToViewport();
	}
}

void ATCOSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATCOSPlayerController, InventoryComponent);
}

void ATCOSPlayerController::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);
	if (IsValid(GetPawn()) && GetPawn()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_EnableGameActions(GetPawn(), true);
	}
}

void ATCOSPlayerController::DisableInput(APlayerController* PlayerController)
{
	Super::DisableInput(PlayerController);
	if (IsValid(GetPawn()) && GetPawn()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_EnableGameActions(GetPawn(), false);
	}
}

void ATCOSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATCOSPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (IsValid(GetTCOSAbilitySystemComponent()))
	{
		TCOSAbilitySystemComp->AbilityInputPressed(InputTag);
	}
}

void ATCOSPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (IsValid(GetTCOSAbilitySystemComponent()))
	{
		TCOSAbilitySystemComp->AbilityInputReleased(InputTag);
	}
}

UTCOSAbilitySystemComponent* ATCOSPlayerController::GetTCOSAbilitySystemComponent()
{
	if (!IsValid(TCOSAbilitySystemComp))
	{
		if (const ATCOSPlayerState* TCOSPlayerState = GetPlayerState<ATCOSPlayerState>())
		{
			TCOSAbilitySystemComp = TCOSPlayerState->GetTCOSAbilitySystemComponent();
		}
	}
	return TCOSAbilitySystemComp;
}