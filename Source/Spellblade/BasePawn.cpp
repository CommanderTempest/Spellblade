// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "SpellBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule collider"));
	RootComponent = CapsuleCom;

	Spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    Spring->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Spring);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("base mesh"));
	BaseMesh->SetupAttachment(CapsuleCom);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("bullet"));
	ProjectileSpawnPoint->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
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
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // binding inputs to functions

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePawn::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePawn::Look);
		EnhancedInputComponent->BindAction(SpellementSelectAction, ETriggerEvent::Triggered, this, &ABasePawn::SelectSpellement);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ABasePawn::Fire);
	}

    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABasePawn::Turn);
}

void ABasePawn::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();

	if (PlayerController && DirectionValue != 0.f)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionValue);
	}
}

// look around using mouse
void ABasePawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ABasePawn::Turn(float Value) {

	AddControllerYawInput(Value);

    // FRotator DeltaRotation = FRotator::ZeroRotator;
    // DeltaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;

    // AddActorLocalRotation(DeltaRotation, true);
}

void ABasePawn::Fire()
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
		
		// ProjectileClass here is whatever Blueprint is set as the projectile on the BP of the pawn
		ASpellBase* Projectile = GetWorld()->SpawnActor<ASpellBase>(ProjectileClass, ProjectileSpawnPointLocation, ProjectileSpawnPointRotation);
		Projectile->SetOwner(this);
		Projectile->SetSpellementType(CurrentlySelectedElement);
	}
	IsFiring = false;
}

void ABasePawn::SelectSpellement(const FInputActionValue& Value)
{
	// for other 2 elements (fire, earth) create a new input modifier
	const float result = Value.Get<float>();
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
	else if (result == 3)
	{
		UE_LOG(LogTemp, Display, TEXT("SELECTED EARTH"));
		CurrentlySelectedElement = ESpellementType::Earth;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BasePawn.cpp: Could not find correct Spellement Type"));
	}
}

