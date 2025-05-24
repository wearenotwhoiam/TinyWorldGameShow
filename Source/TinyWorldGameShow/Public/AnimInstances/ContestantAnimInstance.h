// RnDumb Fun - GameDev.TV Game Jam 2025

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/ContestantBaseAnimInstance.h"
#include "ContestantAnimInstance.generated.h"

class AContestantBaseCharacter;
class UCharacterMovementComponent;

UCLASS()
class TINYWORLDGAMESHOW_API UContestantAnimInstance : public UContestantBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DelatSeconds);

protected:
	UPROPERTY()
	AContestantBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Locomotion")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Locomotion")
	bool bAccelerating;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Locomotion")
	bool bInAir;
};
