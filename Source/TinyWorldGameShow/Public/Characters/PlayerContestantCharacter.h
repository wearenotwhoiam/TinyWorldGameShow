// RnDumb Fun - GameDev.TV Game Jam 2025

#pragma once

#include "Characters/ContestantBaseCharacter.h"
#include "CoreMinimal.h"
#include "PlayerContestantCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
//class UDataAsset_InputConfig;
struct FInputActionValue;

UCLASS()
class TINYWORLDGAMESHOW_API APlayerContestantCharacter : public AContestantBaseCharacter
{
	GENERATED_BODY()

public:
	APlayerContestantCharacter();
protected:
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

private:

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion

#pragma region Inputs

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	//UDataAsset_InputConfig* InputConfigDataAsset;

	//void Input_Move(const FInputActionValue& InputActionValue);
	//void Input_Look(const FInputActionValue& InputActionValue);

#pragma endregion
};
