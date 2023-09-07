// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanEnemyCharacter.h"
#include "Enums.h"
#include "PacmanEnemyController.h"

// Sets default values
APacmanEnemyCharacter::APacmanEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InitialState = EPacmanEnemyState::Spawning;
	OffsetDestination = 300;
	RunAwayDestination = {0,0,0};

}

// Called when the game starts or when spawned
void APacmanEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Controller will be assigned from blueprint
}

// Called every frame
void APacmanEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

