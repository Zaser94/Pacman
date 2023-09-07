// Copyright Epic Games, Inc. All Rights Reserved.

#include "PacmanCharacter.h"

#include "DrawDebugHelpers.h"
#include "PacmanGameMode.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Enums.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Engine/StaticMeshActor.h"

/***********************************************************************
*                     Player Character Class                           *
************************************************************************/
APacmanCharacter::APacmanCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate

	 // Initializing trigger capsule. Final transform set in the blueprint inheritor
	ForwardTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Forward Trigger"));
	ForwardTrigger->InitCapsuleSize(55.f, 96.0f);
	ForwardTrigger->SetCollisionProfileName(TEXT("Trigger"));
	ForwardTrigger->SetupAttachment(RootComponent);
	ForwardTrigger->OnComponentBeginOverlap.AddDynamic(this, &APacmanCharacter::OnOverlapBegin);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TotalScore = 0;

	FString baseAddress = TEXT("AnimSequence'/Game/Animations/Secuences/Player%'");
	const UEnum * enumInstance = FindObject<UEnum>(ANY_PACKAGE,TEXT("EPacmanState"));

	// Saving one animation sequence per enum's element
	for (EPacmanState element : TEnumRange<EPacmanState>())
	{
		FString address = baseAddress;

		// FObjectFinder must be executed in the constructor
		ConstructorHelpers::FObjectFinder<UAnimSequence> animationSequence(*address.Replace(TEXT("%"),*enumInstance->GetNameStringByIndex(static_cast<uint8>(element))));
		AnimSequences.Add(element, animationSequence.Object);
	}

	

}

/***********************************************************************
*                              BEGIN PLAY                              *
************************************************************************/
void APacmanCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode());

	//Global config is set in the game mode
	GetCharacterMovement()->MaxWalkSpeed = GameMode->GetPlayerRunSpeed();

	ChangePacmanState(EPacmanState::Idle);
	PacmanLastState = EPacmanState::Running;

	// Setting collision params for the trace who validates the input
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.bTraceComplex = true;
}

/***********************************************************************
*                              EVENT TICK                              *
************************************************************************/
void APacmanCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if(PacmanState == EPacmanState::Running || PacmanState == EPacmanState::Chasing)
	{
		AddMovementInput(MovementDirection, InputValue);
	}
}

void APacmanCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	DECLARE_DELEGATE_TwoParams(FCustomInputDelegate, const int, const bool);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("MoveUp", IE_Pressed, this, &APacmanCharacter::SetInputMovement, 1, true);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("MoveDown", IE_Pressed, this, &APacmanCharacter::SetInputMovement, -1, true );
	PlayerInputComponent->BindAction<FCustomInputDelegate>("MoveRight", IE_Pressed, this, &APacmanCharacter::SetInputMovement, 1, false);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("MoveLeft", IE_Pressed, this, &APacmanCharacter::SetInputMovement, -1, false);

}

// Preparing the movement by the input given
void APacmanCharacter::SetInputMovement(int Value, bool IsMovingFoward)
{
	if ((Controller != nullptr))
	{
		// find out which way is forward
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
		const FRotationMatrix rotationMatrix = FRotationMatrix(yawRotation);

		const FVector Direction = (IsMovingFoward)? rotationMatrix.GetUnitAxis(EAxis::X)
												  : rotationMatrix.GetUnitAxis(EAxis::Y);
		if(IsInputValid(Direction*Value))
		{
			// Next two lines are a workaround for a bug with the colliding system. TODO: implement a more elegant solution
			ForwardTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetWorldTimerManager().SetTimer(EnablesForwardTriggerHandler, this,
											&APacmanCharacter::EnablesForwardTrigger, 1.0f, false);
			// Set the current movement values
			InputValue = Value;
			MovementDirection = Direction;

			if(PacmanState == EPacmanState::Idle)
			{
				ChangePacmanState(PacmanLastState);
			}
			
		}

	}
}

/* Trace a line to see if there any obstacle from the input direction.
   Basically, player shouldn't go against walls (like in the original pacman) */
bool APacmanCharacter::IsInputValid(FVector Direction)
{
	bool result = false;
	FHitResult validInputTrace;

	// Generates a line trace from player's center
    GetWorld()->LineTraceSingleByChannel(validInputTrace, GetActorLocation() + Direction * GameMode->GetPlayersLocationIncrement(), 
										GetActorLocation() + Direction * GameMode->GetMinDistanceForValidInput(),
										    ECollisionChannel::ECC_WorldStatic, CollisionParams);
#if !UE_BUILD_SHIPPING
    if(GameMode->IsDebugModeActive())
    {
        // Drawing trace camera.
        DrawDebugLine(GetWorld(),
	    GetActorLocation()+ Direction * GameMode->GetPlayersLocationIncrement(),
	    GetActorLocation() + Direction * GameMode->GetMinDistanceForValidInput(),
	    FColor(255, 0, 0),
	    false, 5.0f, 0.0f,5.0f);
    }
#endif

	// If the hit actor is not a static mesh, is a valid direction
	if (!Cast<AStaticMeshActor>(validInputTrace.Actor.Get()))
		return true;

	// if there isn't an impact point, the input is valid
	return validInputTrace.ImpactPoint == FVector::ZeroVector;	
}

// Trigger Begin Overlap
void APacmanCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && Cast<AStaticMeshActor>(OtherActor)) 
	{
		GameMode->PrintDebugMessage("Player hit a wall. Stopping movement.",FColor::Blue,4.0f);
		ChangePacmanState(EPacmanState::Idle);
	}
}

// Handles player's state changes
void APacmanCharacter::ChangePacmanState(EPacmanState NewState)
{

	 PacmanLastState = PacmanState;
	 PacmanState = NewState;

	 // Animation play rates are set in the game mode, for change values fast using the editor.
	 GetMesh()->SetPlayRate(*GameMode->GetAnimationPlayRate(PacmanState));

	 // Playing the proper animation every time player state changes. Death sequence is not played in loop.
	 GetMesh()->PlayAnimation(*AnimSequences.Find(PacmanState), !(PacmanState == EPacmanState::Death));
	 
	 GetCharacterMovement()->MaxWalkSpeed = (PacmanState == EPacmanState::Chasing)? GameMode->GetPlayerChasingSpeed() : GameMode->GetPlayerRunSpeed();
}

void APacmanCharacter::EnablesForwardTrigger()
{
	ForwardTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APacmanCharacter::IncreaseScore_Implementation(float ScoreIncrement)
{
	TotalScore += ScoreIncrement;
	OnScoreIncreased.Broadcast(TotalScore);
	if(TotalScore >= GameMode->GetScoreToWin())
	{
		OnGameVictory.Broadcast();
	}
	
}

void APacmanCharacter::EnterInChasingMode_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = GameMode->GetPlayerChasingSpeed();
	ChangePacmanState(EPacmanState::Chasing);

	GetWorldTimerManager().SetTimer(ExitChasingModeHandler, this,
									&APacmanCharacter::ExitChasingMode, GameMode->GetChasingModeDuration(), false);
}

void APacmanCharacter::ExitChasingMode()
{
	GetCharacterMovement()->MaxWalkSpeed = GameMode->GetPlayerRunSpeed();
	ChangePacmanState(EPacmanState::Running);
}

void APacmanCharacter::Die()
{
	APacmanCharacter::ChangePacmanState(EPacmanState::Death);

	// Disabling input
	APlayerController* playerController = Cast<APlayerController>(GetController());
	DisableInput(playerController);
	OnGameDefeat.Broadcast();
}



       