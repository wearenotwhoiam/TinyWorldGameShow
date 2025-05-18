// RnDumb Fun - GameDev.TV Game Jam 2025


#include "Characters/ContestantBaseCharacter.h"

// Sets default values
AContestantBaseCharacter::AContestantBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;
}
