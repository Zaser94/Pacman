// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PacmanItem.generated.h"

// Forward declarations
class UBoxComponent;
class APacmanGameMode;

UCLASS()
class PACMAN_API APacmanItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APacmanItem();

#pragma region Properties
protected:

	//Reference to the game mode
	APacmanGameMode* GameMode;

	// Item mesh. Initialiased from the editor.
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* ItemMesh = nullptr;

	// Collider who will detect the player
	UPROPERTY(EditAnywhere, Category = "Item")
	UBoxComponent* ItemCollider;

	// Score gived to the player
	UPROPERTY(EditAnywhere, Category = "Item")
	float Score;

#pragma endregion Properties

#pragma region Overrides

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


#pragma endregion Overrides

#pragma region Functions

	// overlap begin function
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#pragma endregion Functions

};
