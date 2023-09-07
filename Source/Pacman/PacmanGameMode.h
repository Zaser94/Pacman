// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacmanGameMode.generated.h"

enum class EPacmanState : uint8;
enum class EPacmanEnemyState : uint8;
class APacmanItem;

UCLASS(minimalapi)
class APacmanGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APacmanGameMode();

protected:

	// Player's default speed
	UPROPERTY(EditDefaultsOnly, Category="Player")
	float PlayerRunSpeed = 400.0f;

	// Player's speed when he gets buffed (entering in chasing mode)
	UPROPERTY(EditDefaultsOnly, Category="Player")
	float PlayerChasingSpeed = 600.0f;

	// Configures player animation play rates
	UPROPERTY(EditDefaultsOnly, Category="Player")
	TMap<EPacmanState, float> PlayerAnimationPlayRates;

	// Enemy's default speed 
	UPROPERTY(EditDefaultsOnly, Category="Enemy")
	float EnemyRunSpeed = 600.0f;

	// Enemy's speed when player gets buffed (entering in chasing mode)
	UPROPERTY(EditDefaultsOnly, Category="Enemy")
	float EnemyAfraidSpeed = 400.0f;

	// Configures enemy's animation play rates
	UPROPERTY(EditDefaultsOnly, Category="Enemy")
	TMap<EPacmanEnemyState, float> EnemyAnimationPlayRates;

	// Configures enemy's animation play rates
	UPROPERTY(EditDefaultsOnly, Category="Enemy")
	TMap<EPacmanEnemyState, float> TotalTimeInEachState;

	// Chasing mode duration
	UPROPERTY(EditDefaultsOnly, Category="Programming Params")
	float ChasingModeDuration = 10.0f;

	// If an input direction collides at this distance with an obstacle: invalid input
	UPROPERTY(EditDefaultsOnly, Category="Programming Params")
	float MinDistanceForValidInput = 70.0f;

	// Distance increment from player's center. Used to determinate the origin of the trace who validates the input.
	UPROPERTY(EditDefaultsOnly, Category="Programming Params")
	float PlayersLocationIncrement = 20.0f;


	// Value of the basic currency
	UPROPERTY(EditDefaultsOnly, Category="Item")
	float BasicScore = 20;

	// Score for killing enemies
	UPROPERTY(EditDefaultsOnly, Category="Item")
	float SpecialScore = 120;

	// Value of the power up
	UPROPERTY(EditDefaultsOnly, Category="Item")
	float PowerUpScore = 60;

	// Win condition
	UPROPERTY(EditDefaultsOnly, Category="Item")
	float ScoreToWin = 1000;

	// Enables debugging info 
	UPROPERTY(EditDefaultsOnly, Category="Debug")
	bool bIsDebugModeActive = true;




public:
	FORCEINLINE float  GetPlayerRunSpeed() const { return PlayerRunSpeed; }
	FORCEINLINE float  GetPlayerChasingSpeed() const { return PlayerChasingSpeed; }
	FORCEINLINE float* GetAnimationPlayRate(EPacmanState state) { return PlayerAnimationPlayRates.Find(state); }
	FORCEINLINE float  GetEnemyRunSpeed() const { return EnemyRunSpeed; }
	FORCEINLINE float  GetEnemyAfraidSpeed() const { return EnemyAfraidSpeed; }
	FORCEINLINE float* GetAnimationPlayRate(EPacmanEnemyState state) { return EnemyAnimationPlayRates.Find(state); }
	FORCEINLINE float* GetTotalTimeInEachState(EPacmanEnemyState state) { return TotalTimeInEachState.Find(state); }
	FORCEINLINE float  GetMinDistanceForValidInput() { return MinDistanceForValidInput; }
	FORCEINLINE float  GetPlayersLocationIncrement() { return PlayersLocationIncrement; }
	FORCEINLINE float  GetChasingModeDuration() { return ChasingModeDuration; }
	FORCEINLINE float  GetBasicScore() { return BasicScore; }
	FORCEINLINE float  GetSpecialScore() { return SpecialScore; }
	FORCEINLINE float  GetPowerUpScore() { return PowerUpScore; }
	FORCEINLINE float  GetScoreToWin() { return ScoreToWin; }

	FORCEINLINE bool  IsDebugModeActive() { return bIsDebugModeActive; }


	void PrintDebugMessage(FString message, FColor color, float timeOnScreen);

	// When this list gets empty, player wins
	TArray<APacmanItem>* AllItemsFromLevel;
};



