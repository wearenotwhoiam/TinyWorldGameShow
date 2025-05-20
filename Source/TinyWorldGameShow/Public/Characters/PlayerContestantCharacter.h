// RnDumb Fun - GameDev.TV Game Jam 2025

#pragma once

#include "Characters/ContestantBaseCharacter.h"
#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "PlayerContestantCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;


UCLASS()
class TINYWORLDGAMESHOW_API APlayerContestantCharacter : public AContestantBaseCharacter
{
	GENERATED_BODY()

public:
	APlayerContestantCharacter();

	IOnlineSessionPtr OnlineSessionInterface;
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
private:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_CreateGameSession(const FInputActionValue& InputActionValue);
	void Input_JoinGameSession(const FInputActionValue& InputActionValue);
#pragma endregion

#pragma region Delegates
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
#pragma endregion

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
