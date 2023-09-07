// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanPowerUp.h"

#include "PacmanGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ItemInteractInterface.h"

APacmanPowerUp::APacmanPowerUp()
{
	
}

void APacmanPowerUp::BeginPlay()
{
	Super::BeginPlay();
	Score = GameMode->GetPowerUpScore();
}

void APacmanPowerUp::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) 
	{
		 // if the actor who enters in the collider implements this interface, execute the logic.
		 if(UKismetSystemLibrary::DoesImplementInterface(OtherActor, UItemInteractInterface::StaticClass()))
		 {
		 	 GameMode->PrintDebugMessage("Player enters in Chasing Mode", FColor::Red, 4.0f);
			 IItemInteractInterface::Execute_EnterInChasingMode(OtherActor);
			 // power ups also give score to the player
			 Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		 }

	}
}