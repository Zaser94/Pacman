// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PacmanEnemyCharacter.generated.h"


// Forward declaractions
enum class EPacmanEnemyState: uint8;
class APacmanEnemyController;

UCLASS()
class PACMAN_API APacmanEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APacmanEnemyCharacter();

	UPROPERTY(EditDefaultsOnly, Category="State")
	EPacmanEnemyState InitialState;

	UPROPERTY(EditDefaultsOnly, Category="Destination")
	float OffsetDestination;

	UPROPERTY(EditDefaultsOnly, Category="Destination")
	FVector RunAwayDestination;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
