// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PacmanEnemy.generated.h"

UCLASS()
class PACMAN_API APacmanEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APacmanEnemy();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
