// RnDumb Fun - GameDev.TV Game Jam 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

//maps input tag to input action
USTRUCT(BlueprintType)
struct FContestantInputActionConfig
{
	GENERATED_BODY();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;
};

UCLASS()
class TINYWORLDGAMESHOW_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FContestantInputActionConfig> NativeInputActions;

	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;
};
