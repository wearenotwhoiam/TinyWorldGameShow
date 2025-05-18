// RnDumb Fun - GameDev.TV Game Jam 2025

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"

#include "DataAssets/Input/DataAsset_InputConfig.h"

#include "ContestantInputComponent.generated.h"

/**
 *
 */
UCLASS()
class TINYWORLDGAMESHOW_API UContestantInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(
		const UDataAsset_InputConfig* InInputConfig,
		const FGameplayTag& InInputTag,
		ETriggerEvent TriggerEvent,
		UserObject* ContextObject,
		CallbackFunc Func
	);

};

template<class UserObject, typename CallbackFunc>
inline void UContestantInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	checkf(InInputConfig, TEXT("Input config data asset is null cannot bind"));

	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}
