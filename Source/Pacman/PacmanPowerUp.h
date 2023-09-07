// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacmanItem.h"
#include "PacmanPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API APacmanPowerUp : public APacmanItem
{
	GENERATED_BODY()

public:
	APacmanPowerUp();

protected:
	virtual void BeginPlay() override;
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
