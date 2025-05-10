// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/TCOSInputConfig.h"
#include "TCOSSystemInputComponent.generated.h"

class UTCOSInputConfig;
/**
 * 
 */
UCLASS()
class TCOS_MULTIPLAYER_API UTCOSSystemInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType> 
	void BindAbilityActions(UTCOSInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);
	
	
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UTCOSSystemInputComponent::BindAbilityActions(UTCOSInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const FTCOSInputAction& Action : InputConfig->TCOSInputActions)
	{
		if (IsValid(Action.InputAction) && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
		}
	}
}
