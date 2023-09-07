// Copyright Epic Games, Inc. All Rights Reserved.

#include "PacmanGameMode.h"
#include "PacmanCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Enums.h"

APacmanGameMode::APacmanGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerAnimationPlayRates.Add (EPacmanState::Idle, 1.0f);
	PlayerAnimationPlayRates.Add (EPacmanState::Running, 1.0f);
	PlayerAnimationPlayRates.Add (EPacmanState::Chasing, 1.0f);
	PlayerAnimationPlayRates.Add (EPacmanState::Death, 1.0f);

	EnemyAnimationPlayRates.Add(EPacmanEnemyState::Spawning, 1.0f);
	EnemyAnimationPlayRates.Add(EPacmanEnemyState::Chasing, 1.0f);
	EnemyAnimationPlayRates.Add(EPacmanEnemyState::Running, 1.0f);
	EnemyAnimationPlayRates.Add(EPacmanEnemyState::Afraid, 1.0f);
	EnemyAnimationPlayRates.Add(EPacmanEnemyState::Death, 1.0f);

	TotalTimeInEachState.Add(EPacmanEnemyState::Spawning, 3.0f);
	TotalTimeInEachState.Add(EPacmanEnemyState::Chasing, 20.0f);
	TotalTimeInEachState.Add(EPacmanEnemyState::Running, 10.0f);
	TotalTimeInEachState.Add(EPacmanEnemyState::Afraid, 10.0f);
	TotalTimeInEachState.Add(EPacmanEnemyState::Death, 5.0f);

}

void APacmanGameMode::PrintDebugMessage(FString Message, FColor Color, float TimeOnScreen)
{
#if !UE_BUILD_SHIPPING

	if (IsDebugModeActive() && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeOnScreen, Color, *Message);
	}

#endif

}


