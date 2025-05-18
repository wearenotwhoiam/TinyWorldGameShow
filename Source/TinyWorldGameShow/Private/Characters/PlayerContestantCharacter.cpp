// RnDumb Fun - GameDev.TV Game Jam 2025

#include "Characters/PlayerContestantCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
//#include "EnhancedInputSubsystems.h"
//#include "DataAssets/Input/DataAsset_InputConfig.h"
//#include "Components/Input/ContestantInputComponent.h"
//#include "ContestantGameplayTags.h"
#include "ContestantDebugHelper.h"


APlayerContestantCharacter::APlayerContestantCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 100.f);

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
}

void APlayerContestantCharacter::BeginPlay()
{
	Super::BeginPlay();
	Debug::Print(TEXT("Begun"));
}