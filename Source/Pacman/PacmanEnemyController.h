// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PacmanEnemyController.generated.h"

// Forward declarations
enum class EPacmanEnemyState: uint8;
class APacmanEnemyCharacter;
class APacmanGameMode;
class APacmanCharacter;

UCLASS()
class PACMAN_API APacmanEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	APacmanEnemyController();
protected:
#pragma region Properties

	// A reference to the current game mode
	APacmanGameMode* GameMode;

	UPROPERTY(BlueprintReadWrite)
	// State machine that governs enemy behaviour
	EPacmanEnemyState EnemyState;

	// Stores controlled pawn
	APacmanEnemyCharacter* ControlledPawn;

	// Player's reference
	 APacmanCharacter* Player;

	//Store each anim sequence 
	TMap<EPacmanEnemyState,UAnimSequence*> AnimSequences;

	void BeginPlay() override;
	void Tick(float delta) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;

	UFUNCTION(BlueprintCallable)
	void ChangeEnemyState(EPacmanEnemyState NewState);

	void MovingToNearRandomLocation();

	FTimerHandle ExitStateHandler;
	FTimerDelegate ExitStateDelegate;

	UFUNCTION(BlueprintCallable)
	void Die();
	
};
