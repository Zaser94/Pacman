// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemInteractInterface.h"
#include "PacmanCharacter.generated.h"

// Forward declarations 
class APacmanGameMode;
class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;
enum class EPacmanState : uint8;

UCLASS(config=Game)
class APacmanCharacter : public ACharacter,
						 public IItemInteractInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Triggers")
	UCapsuleComponent* ForwardTrigger;


public:
	APacmanCharacter();

	// Delegate to update score in HUD widget
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreIncreased, const float, Score);
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
    FOnScoreIncreased OnScoreIncreased;

	// Delegate to end game, lose condition
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameDefeat);
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
    FOnGameDefeat OnGameDefeat;

	// Delegate to end game, win condition
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameVictory);
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
    FOnGameVictory OnGameVictory;

protected:

	// A reference to the current game mode
	APacmanGameMode* GameMode;

	UPROPERTY(BlueprintReadWrite)
	// Controls animations and player behaviour
	EPacmanState PacmanState;

	// Useful to return from Idle to Chasing or Running
	EPacmanState PacmanLastState;

	int TotalScore;

	// Direction of the player during movement
	FVector MovementDirection;

	// Allow movement between inputs
	bool IsMovingForward;

	//Each movement has two opposites 
	short InputValue;

	//Store each anim sequence 
	TMap<EPacmanState,UAnimSequence*> AnimSequences;

	// Collision params for tracing obstacles
	FCollisionQueryParams CollisionParams;

	// Prepares and start movement
	UFUNCTION()
	void SetInputMovement(int Value, bool IsMovingFoward);

	// Validating the input to improve the gameplay 
	bool IsInputValid(FVector Direction);

	// Change state and animation
	UFUNCTION(BlueprintCallable)
	void ChangePacmanState(EPacmanState newState);

	// APawn override to initialise binding inputs
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	// IItemInteractInterface functions:
	void IncreaseScore(float ScoreIncrement);  
    virtual void IncreaseScore_Implementation(float ScoreIncrement) override;

	void EnterInChasingMode();  
    virtual void EnterInChasingMode_Implementation() override;

	// overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle EnablesForwardTriggerHandler;
	UFUNCTION()
	void EnablesForwardTrigger();

	FTimerHandle ExitChasingModeHandler;
	UFUNCTION()
	void ExitChasingMode();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Get player state.
	//UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
	FORCEINLINE EPacmanState GetPlayerState() const { return PacmanState; }

	UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
	FORCEINLINE int GetTotalScore() const { return TotalScore; }

	FORCEINLINE FVector GetFowardVector() const { return MovementDirection; }

	UFUNCTION(BlueprintCallable)
	void Die();

};

