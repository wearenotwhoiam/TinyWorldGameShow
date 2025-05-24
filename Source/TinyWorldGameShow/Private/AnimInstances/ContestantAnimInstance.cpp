// RnDumb Fun - GameDev.TV Game Jam 2025


#include "AnimInstances/ContestantAnimInstance.h"
#include "Characters/ContestantBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UContestantAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AContestantBaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
};
void UContestantAnimInstance::NativeThreadSafeUpdateAnimation(float DelatSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent) return;
	
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	bInAir = OwningCharacter->GetCharacterMovement()->IsFalling();
	bAccelerating = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}