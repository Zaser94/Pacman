// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInteractInterface : public UInterface
{
	GENERATED_BODY()
};

// This Interface represent all the modifiers that items can apply to the player
// NOTE: If I have enough time, I will rename it (what an awful name hehe)
class PACMAN_API IItemInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void IncreaseScore(float ScoreIncrement);

	UFUNCTION(BlueprintNativeEvent)
	void EnterInChasingMode();
};
