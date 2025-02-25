// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "CollisionDebugDrawingPublic.h"
#include "SpellBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectileSpell.h"
#include "WallSpell.h"
#include "Evaluation/IMovieSceneEvaluationHook.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CapsuleCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule collider"));
	// RootComponent = CapsuleCom;

	Spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Spring->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Spring);
	//
	// BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("base mesh"));
	// BaseMesh->SetupAttachment(CapsuleCom);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// binding inputs to functions

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		EnhancedInputComponent->BindAction(SpellementSelectAction, ETriggerEvent::Started, this, &ABaseCharacter::SelectSpellement);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABaseCharacter::Fire);
		EnhancedInputComponent->BindAction(StrafeAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Strafe);
		EnhancedInputComponent->BindAction(CastTypeAction, ETriggerEvent::Started, this, &ABaseCharacter::SelectCastType);
	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();

	if (PlayerController && DirectionValue != 0.f)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionValue);
	}
}

// look around using mouse
void ABaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ABaseCharacter::Strafe(const FInputActionValue& Value) {

	const float DirectionValue = Value.Get<float>();

	if (PlayerController && DirectionValue != 0.f)
	{
		FVector Left = GetActorRightVector();
		AddMovementInput(Left, DirectionValue);
	}
}

void ABaseCharacter::Fire(const FInputActionValue& Value)
{
	FVector ProjectileSpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ProjectileSpawnPointRotation = ProjectileSpawnPoint->GetComponentRotation();
	FRotator CameraRotation = Camera->GetComponentRotation();

	const FVector InputValue = Value.Get<FVector>();

	// DrawDebugSphere(
	// 	GetWorld(),
	// 	ProjectileSpawnPointLocation,
	// 	25.f,
	// 	12,
	// 	FColor::Red,
	// 	false,
	// 	3.f
	// );
	
	// ProjectileClass here is whatever Blueprint is set as the projectile on the BP of the character
	if (CastType == ECastType::Projectile)
	{
		AProjectileSpell::FireSpell(GetWorld(), GetOwner(), ProjectileClass, ProjectileSpawnPointLocation, CameraRotation, CurrentlySelectedElement);
	}
	else if (CastType == ECastType::Wall)
	{
		// Fire wall spell

		//FVector WallSpawnLocation;
		//FRotator WallSpawnRotation;

		// Another idea might be for Scroll Wheel on walls to increase distance from player,
		// at 0 distance could do a ramp?
		
		FVector WallSpawnLoc = GetActorLocation() + (GetActorForwardVector() * 300);
		if (PlayerController)
		{
			AWallSpell::FireSpell(GetWorld(), GetOwner(), WallClass, WallSpawnLoc, GetActorRotation(), CurrentlySelectedElement);
		}
	}
	else {UE_LOG(LogTemp, Error, TEXT("Cast Type not set!"));}
}

void ABaseCharacter::SelectSpellement(const FInputActionValue& Value)
{
	const float result = Value.Get<float>();
	UE_LOG(LogTemp, Display, TEXT("%f"), result);
	if (result == 1)
	{
		CurrentlySelectedElement = ESpellementType::Wind;
	}
	else if (result == -1)
	{
		CurrentlySelectedElement = ESpellementType::Water;
	}
	else if (result == 2)
	{
		UE_LOG(LogTemp, Display, TEXT("SELECTED FIRE"));
		CurrentlySelectedElement = ESpellementType::Fire;
	}
	else if (result == -2)
	{
		UE_LOG(LogTemp, Display, TEXT("SELECTED EARTH"));
		CurrentlySelectedElement = ESpellementType::Earth;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BasePawn.cpp: Could not find correct Spellement Type"));
	}
}

void ABaseCharacter::SelectCastType(const FInputActionValue& Value)
{
	// need to add some UI to visibly show this
	if (CastType == ECastType::Projectile)
	{
		CastType = ECastType::Wall;
		UE_LOG(LogTemp, Display, TEXT("Switched cast-type to Wall Spell"));
	}
	else if (CastType == ECastType::Wall)
	{
		CastType = ECastType::Projectile;
		UE_LOG(LogTemp, Display, TEXT("Switched cast-type to Projectile Spell"));
	}
	else if (CastType == ECastType::None)
	{
		CastType = ECastType::Projectile;
		UE_LOG(LogTemp, Display, TEXT("Switched cast-type to Projectile Spell"));
	}
}

