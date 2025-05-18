// RnDumb Fun - GameDev.TV Game Jam 2025

#pragma once

#include "CoreMinimal.h"
#include "Characters/ContestantBaseCharacter.h"
#include "PlayerContestant.generated.h"

/**
 * 
 */
UCLASS()
class TINYWORLDGAMESHOW_API APlayerContestant : public AContestantBaseCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
