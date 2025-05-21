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

#include "Components/WidgetComponent.h"


APlayerContestantCharacter::APlayerContestantCharacter()
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

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.f,
	//		FColor::Cyan,
	//		FString::Printf(TEXT("Session: %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
	//}
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
}

void APlayerContestantCharacter::BeginPlay()
{
	Super::BeginPlay();
}

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
