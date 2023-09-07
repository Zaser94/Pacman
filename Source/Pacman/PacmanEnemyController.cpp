// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanEnemyController.h"
#include "Enums.h"
#include "PacmanEnemyCharacter.h"
#include "PacmanGameMode.h"
#include "PacmanCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

APacmanEnemyController::APacmanEnemyController()
{

	FString baseAddress = TEXT("AnimSequence'/Game/Animations/Secuences/Enemy%'");
	const UEnum * enumInstance = FindObject<UEnum>(ANY_PACKAGE,TEXT("EPacmanEnemyState"));

	// Initializing animations by the enemy state enum
	for (EPacmanEnemyState element : TEnumRange<EPacmanEnemyState>())
	{
		// Running state has the same animation than Chasing (the enemy just run away instead of chases the player)
		if(element == EPacmanEnemyState::Running)
		{
			element = EPacmanEnemyState::Chasing;
		}

		FString address = baseAddress;

		// FObjectFinder must be executed in the constructor
		ConstructorHelpers::FObjectFinder<UAnimSequence> animationSequence(*address.Replace(TEXT("%"),*enumInstance->GetNameStringByIndex(static_cast<uint8>(element))));
		AnimSequences.Add(element, animationSequence.Object);
	}
}

void APacmanEnemyController::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode());
	Player = Cast<APacmanCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ControlledPawn = Cast<APacmanEnemyCharacter>(GetPawn());
	ChangeEnemyState(ControlledPawn->InitialState);
				
}

void APacmanEnemyController::Tick(float delta)
{
	Super::Tick(delta);
}

void APacmanEnemyController::ChangeEnemyState(EPacmanEnemyState NewState)
{
	 EnemyState = NewState;

	 // Animation play rates are set in the game mode, for change values fast using the editor.
	 ControlledPawn->GetMesh()->SetPlayRate(*GameMode->GetAnimationPlayRate(EnemyState));
	 EPacmanEnemyState auxState =  EnemyState == EPacmanEnemyState::Running? EPacmanEnemyState::Chasing: EnemyState;
	 // Playing the proper animation every time player state changes. Death sequence and spawning is not played in loop.
	 ControlledPawn->GetMesh()->PlayAnimation(*AnimSequences.Find(auxState), !(EnemyState == EPacmanEnemyState::Death ||EnemyState == EPacmanEnemyState::Spawning));

	 EPacmanEnemyState nextState;
	 ControlledPawn->GetCharacterMovement()->MaxWalkSpeed =  GameMode->GetEnemyRunSpeed();
	 // State machine flow
	 switch (EnemyState)
	 {
		 case EPacmanEnemyState::Spawning : 
		 {
			 nextState = EPacmanEnemyState::Chasing;
		 }
			break;
		 case EPacmanEnemyState::Chasing : 
		 {
			 StopMovement();
		 	 MoveToLocation(Player->GetActorLocation() + Player->GetFowardVector()*ControlledPawn->OffsetDestination);
			 nextState = EPacmanEnemyState::Running;
		 }
		break;
		case EPacmanEnemyState::Running : 
		{
			 StopMovement();
			 MoveToLocation(ControlledPawn->RunAwayDestination);
			 nextState = EPacmanEnemyState::Chasing;
		}
		break;
		case EPacmanEnemyState::Afraid : 
		{
			 StopMovement();
			 ControlledPawn->GetCharacterMovement()->MaxWalkSpeed =  GameMode->GetEnemyAfraidSpeed();
			 MovingToNearRandomLocation();
			 nextState = EPacmanEnemyState::Chasing;

		}
		break;
		case EPacmanEnemyState::Death :
		{
			 StopMovement();
			 nextState = EPacmanEnemyState::Spawning;
		}
		break;
	 }

	 // Calling again this function X seconds later depending on the current state duration
	 ExitStateDelegate = FTimerDelegate::CreateUObject( this, &APacmanEnemyController::ChangeEnemyState, nextState );
	 GetWorldTimerManager().SetTimer(ExitStateHandler,ExitStateDelegate, *GameMode->GetTotalTimeInEachState(EnemyState), false);
}

void APacmanEnemyController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if(Result != EPathFollowingResult::Type::Aborted)
	{
		if(EnemyState == EPacmanEnemyState::Chasing)
		{
			// Continue chasing the player with an offset desviation from player's location
			MoveToLocation(Player->GetActorLocation()+ Player->GetFowardVector()*ControlledPawn->OffsetDestination);
		}
		else
		if(EnemyState == EPacmanEnemyState::Running)
		{
			// Running away from the player
			MoveToLocation(ControlledPawn->RunAwayDestination);
		}
		else
		if(EnemyState == EPacmanEnemyState::Afraid)
		{
			MovingToNearRandomLocation();
		}
	}

}

void APacmanEnemyController::MovingToNearRandomLocation()
{
			bool negativeNumber = FMath::RandBool();
		float i = negativeNumber? 1.0f :-1.0f;
		FVector direction = {i,0,0};
		int j = FMath::RandRange(1,3);
		switch (j)
		{
			case 1: direction = {i,0,0};
				break;
			case 2: direction = {0,i,0};
				break;
			case 3: direction = {0,0,i};
				break;
		}
		float randomOffset = FMath::RandRange(100,500);

		// Moving to a random near location  
		MoveToLocation(ControlledPawn->GetActorLocation() + direction * randomOffset);
}

void APacmanEnemyController::Die()
{
	ChangeEnemyState(EPacmanEnemyState::Death);
	
	IItemInteractInterface::Execute_IncreaseScore(Player, GameMode->GetSpecialScore());
}