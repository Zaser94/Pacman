// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanItem.h"

#include <string>

#include "Components/BoxComponent.h"
#include "PacmanGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ItemInteractInterface.h"
#include "PacmanCharacter.h"

// Sets default values
APacmanItem::APacmanItem()
{
	// declare trigger capsule. Final transform set in the blueprint inheritor
	ItemCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Item Trigger"));
	ItemCollider->InitBoxExtent({ 50.f,50.f,50.f });
	ItemCollider->SetCollisionProfileName(TEXT("Trigger"));
	SetRootComponent(ItemCollider);
	ItemCollider->OnComponentBeginOverlap.AddDynamic(this, &APacmanItem::OnOverlapBegin);

	// item mesh initialization. Static mesh asigned in the blueprint inheritor
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void APacmanItem::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode());

	// Adding this item to the list
	//GameMode->AllItemsFromLevel->Add(*this);

	// Basic Score
	Score = GameMode->GetBasicScore();
}


// Trigger Begin Overlap
void APacmanItem::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) 
	{
		 // if the actor who enters in the collider implements this interface, execute the logic.
		 if(UKismetSystemLibrary::DoesImplementInterface(OtherActor, UItemInteractInterface::StaticClass()))
		 {
		 	 GameMode->PrintDebugMessage("Player gets item "+GetName(), FColor::Blue, 4.0f);
			 IItemInteractInterface::Execute_IncreaseScore(OtherActor, Score);
			 Destroy();
		 }

	}
}