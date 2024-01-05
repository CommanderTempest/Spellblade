// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "SpellBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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
		EnhancedInputComponent->BindAction(SpellementSelectAction, ETriggerEvent::Triggered, this, &ABaseCharacter::SelectSpellement);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Fire);
		EnhancedInputComponent->BindAction(StrafeAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Strafe);
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

void ABaseCharacter::Fire()
{
	if (IsFiring == false)
	{
		IsFiring = true;
		FVector ProjectileSpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator ProjectileSpawnPointRotation = ProjectileSpawnPoint->GetComponentRotation();

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
		if (ProjectileClass)
		{
			ASpellBase* Projectile = GetWorld()->SpawnActor<ASpellBase>(ProjectileClass, ProjectileSpawnPointLocation, ProjectileSpawnPointRotation);
			Projectile->SetOwner(this);
			Projectile->SetSpellementType(CurrentlySelectedElement);
		}
		else {UE_LOG(LogTemp, Error, TEXT("Projectile Class not set!"));}
	}
	IsFiring = false;
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

