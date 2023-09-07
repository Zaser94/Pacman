#pragma once

#include "CoreMinimal.h"

// Used for the state machine that controls player's movement and animations
UENUM()
enum class EPacmanState: uint8
{
	Idle		UMETA(DisplayName="Idle"),
	Running		UMETA(DisplayName="Running"),
	Chasing		UMETA(DisplayName="Chasing"),
	Death		UMETA(DisplayName="Death"),
	None        UMETA(DisplayName="None")
};

ENUM_RANGE_BY_COUNT(EPacmanState, EPacmanState::None);

// Used for the state machine that controls enemies' movement and animations
UENUM()
enum class EPacmanEnemyState: uint8
{
	Spawning	UMETA(DisplayName="Spawning"),
	Chasing		UMETA(DisplayName="Chasing"),
	Running		UMETA(DisplayName="Running"),
	Afraid		UMETA(DisplayName="Afraid"),
	Death		UMETA(DisplayName="Death"),
	None        UMETA(DisplayName="None")
};

ENUM_RANGE_BY_COUNT(EPacmanEnemyState, EPacmanEnemyState::None);