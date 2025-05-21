// RnDumb Fun - GameDev.TV Game Jam 2025


#include "GameModes/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!GameState) return;

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage
		(
			1,
			60.f,
			FColor::Yellow,
			FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers)
		);

		APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
		if (PlayerState)
		{
			FString PlayerName = PlayerState->GetPlayerName();
			GEngine->AddOnScreenDebugMessage
			(
				-1,
				60.f,
				FColor::Cyan,
				FString::Printf(TEXT("%s has joind the game"), *PlayerName)
			);
		}

		if (NumberOfPlayers == 2)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				bUseSeamlessTravel = true;
				World->ServerTravel(FString("/Game/Maps/WaterTest?listen"));
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		GEngine->AddOnScreenDebugMessage
		(
			1,
			60.f,
			FColor::Yellow,
			FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers - 1)
		);

		FString PlayerName = PlayerState->GetPlayerName();
		GEngine->AddOnScreenDebugMessage
		(
			-1,
			60.f,
			FColor::Cyan,
			FString::Printf(TEXT("%s has exited the game"), *PlayerName)
		);
	}
}