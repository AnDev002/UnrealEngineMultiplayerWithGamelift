// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignInOverlay.generated.h"

class UJoinGame;
class UPortalManager;
class UGameSessionsManager;
class UWidgetSwitcher;
class USignInPage;
class UConfirmSignUpPage;
class USignUpPage;
class USuccessConfirmedPage;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API USignInOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameSessionsManager> GameSessionsManagerClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignInPage> Page_SignIn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignUpPage> Page_SignUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UConfirmSignUpPage> Page_ConfirmSignUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USuccessConfirmedPage> Page_SuccessConfirmed;

	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	UPROPERTY()
	TObjectPtr<UGameSessionsManager> GameSessionsManager;

	UFUNCTION()
	void ShowSignInPage();

	UFUNCTION()
	void ShowSignUpPage();

	UFUNCTION()
	void ShowConfirmSignUpPage();

	UFUNCTION()
	void ShowSuccessConfirmedPage();

	UFUNCTION()
	void SignInButtonClicked();

	UFUNCTION()
	void SignUpButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

	UFUNCTION()
	void ConfirmSignUpButtonClicked();

	UFUNCTION()
	void OkButtonClicked();

	UFUNCTION()
	void OnSignUpSucceeded();

	UFUNCTION()
	void OnConfirmSucceeded();
};
