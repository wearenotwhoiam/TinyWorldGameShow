// RnDumb Fun - GameDev.TV Game Jam 2025

#include "Characters/PlayerContestantCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/ContestantInputComponent.h"

#include "ContestantGameplayTags.h"
#include "Kismet/GameplayStatics.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "ContestantDebugHelper.h"


APlayerContestantCharacter::APlayerContestantCharacter() : 
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnCreateSessionComplete)),
	FindSessionCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,&ThisClass::OnFindSessionComplete))
{
	GetCapsuleComponent()->InitCapsuleSize(120.f, 100.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 65.f, 80.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) return;
	OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			FString::Printf(TEXT("Session: %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
	}
}

void  APlayerContestantCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));
	ULocalPlayer* LocalPlayer{ GetController<APlayerController>()->GetLocalPlayer() };

	UEnhancedInputLocalPlayerSubsystem* Subsystem{ ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer) };

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UContestantInputComponent* ContestantInputComponent{ CastChecked<UContestantInputComponent>(PlayerInputComponent) };
	ContestantInputComponent->BindNativeInputAction(InputConfigDataAsset, ContestantGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	ContestantInputComponent->BindNativeInputAction(InputConfigDataAsset, ContestantGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	ContestantInputComponent->BindNativeInputAction(InputConfigDataAsset, ContestantGameplayTags::InputTag_OpenLobby, ETriggerEvent::Started, this, &ThisClass::Input_OpenLobby);
	ContestantInputComponent->BindNativeInputAction(InputConfigDataAsset, ContestantGameplayTags::InputTag_CallOpenLevel, ETriggerEvent::Started, this, &ThisClass::Input_CallOpenLevel);
	ContestantInputComponent->BindNativeInputAction(InputConfigDataAsset, ContestantGameplayTags::InputTag_CallClientTravel, ETriggerEvent::Started, this, &ThisClass::Input_CallClientTravel);
	ContestantInputComponent->BindNativeInputAction(InputConfigDataAsset, ContestantGameplayTags::InputTag_CreateGameSession, ETriggerEvent::Started, this, &ThisClass::Input_CreateGameSession);
	ContestantInputComponent->BindNativeInputAction(InputConfigDataAsset, ContestantGameplayTags::InputTag_JoinGameSession, ETriggerEvent::Started, this, &ThisClass::Input_JoinGameSession);
}

void APlayerContestantCharacter::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region Input
void APlayerContestantCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector{ InputActionValue.Get<FVector2D>() };
	const FRotator MovementRotation{ 0.f, Controller->GetControlRotation().Yaw, 0.f };

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection{ MovementRotation.RotateVector(FVector::ForwardVector) };

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection{ MovementRotation.RotateVector(FVector::RightVector) };

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerContestantCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector{ InputActionValue.Get<FVector2D>() };

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerContestantCharacter::Input_OpenLobby(const FInputActionValue& InputActionValue)
{
	Debug::Print(TEXT("OpenLobby"));

	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Content/Maps/FeatureDevMap?listen");
	}
}

void APlayerContestantCharacter::Input_CallOpenLevel(const FInputActionValue& InputActionValue)
{
	Debug::Print(TEXT("CallOpenLevel"));
	UGameplayStatics::OpenLevel(this, "192.168.1.91");
}

void APlayerContestantCharacter::Input_CallClientTravel(const FInputActionValue& InputActionValue)
{
	Debug::Print(TEXT("CallClientTravel"));

	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PlayerController) return;

	PlayerController->ClientTravel("192.168.1.91", ETravelType::TRAVEL_Absolute);
}

void APlayerContestantCharacter::Input_CreateGameSession(const FInputActionValue& InputActionValue)
{
	if (!OnlineSessionInterface.IsValid()) return;

	//Check for existing session - if one exists->DESTROOOOOY
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}

	////Add delegate to game session's delegate list
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = false;
	SessionSettings->NumPublicConnections = 4;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void APlayerContestantCharacter::Input_JoinGameSession(const FInputActionValue& InputActionValue)
{
	if (!OnlineSessionInterface.IsValid()) return;

	Debug::Print(TEXT("JoinSession"));

	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}
#pragma endregion

#pragma region Delegates
void APlayerContestantCharacter::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 
			15.f, 
			FColor::Cyan, 
			FString::Printf(TEXT("Session: %s"), *SessionName.ToString()));

		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/Content/Maps/LobbyLevel?listen");
		}
	}
	else
	{
		Debug::Print(TEXT("Failed to create session"));
	}
}

void APlayerContestantCharacter::OnFindSessionComplete(bool bWasSuccessful)
{
	Debug::Print(TEXT("FindSessionComplete"));
	for (auto Session : SessionSearch->SearchResults)
	{
		Debug::Print(TEXT("Session"));
		FString ID = Session.GetSessionIdStr();
		FString User = Session.Session.OwningUserName;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("ID: %s, User: %s"), *ID, *User));
		}
	}
}
#pragma endregion
