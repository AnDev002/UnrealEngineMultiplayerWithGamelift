// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DSPlayerController.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/InventoryInterface.h"
#include "GameplayTagContainer.h"
#include "TCOSPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuitMenuOpen, bool, bOpen);

class UInventoryComponent;
class UInventoryWidgetController;
class UTCOSSystemsWidget;
class UTCOSInputConfig;
class UTCOSAbilitySystemComponent;

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class TCOS_MULTIPLAYER_API ATCOSPlayerController : public ADSPlayerController, public IAbilitySystemInterface, public IInventoryInterface
{
	GENERATED_BODY()
	
public:
	ATCOSPlayerController();

	virtual void SetupInputComponent() override;

	/*Implement Inventory Interface*/
	virtual UInventoryComponent* GetInventoryComponent_Implementation() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UInventoryWidgetController* GetInventoryWidgetController();

	UFUNCTION(BlueprintCallable)
	void CreateInventoryWidget();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void EnableInput(class APlayerController* PlayerController) override;
	virtual void DisableInput(class APlayerController* PlayerController) override;

	UPROPERTY(BlueprintAssignable)
	FOnQuitMenuOpen OnQuitMenuOpen;
protected:
	virtual void BeginPlay() override;
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
private:
	UPROPERTY()
	TObjectPtr<UTCOSAbilitySystemComponent> TCOSAbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Input")
	TObjectPtr<UTCOSInputConfig> TCOSInputConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Replicated)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	
	UPROPERTY(EditDefaultsOnly, Category = "Custom Value|Widgets")
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UTCOSSystemsWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Value|Widgets")
	TSubclassOf<UTCOSSystemsWidget> InventoryWidgetClass;

	UTCOSAbilitySystemComponent* GetTCOSAbilitySystemComponent();

};
