// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SpellementType.h"
#include "BaseCharacter.generated.h"


/*
This is the character header file
*/

class UInputMappingContext;
class UInputAction;

UENUM()
enum class ECastType
{
	Projectile UMETA(DisplayName = "Projectile"),
	Wall UMETA(DisplayName = "Wall"), 
	Field UMETA(DisplayName = "Field"), 
	None UMETA(DisplayName="None")
};

UCLASS()
class SPELLBLADE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// *** INPUTS ***

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* StrafeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SpellementSelectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CastTypeAction;

	void Fire(const FInputActionValue& Value); 
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Strafe(const FInputActionValue& Value);
	void SelectSpellement(const FInputActionValue& Value);
	void SelectCastType(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	APlayerController* GetPlayerController() const {return PlayerController;}

	float GetMana() {return Mana;}
	void SetMana(float CMana) {Mana=CMana;}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ASpellBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AWallSpell> WallClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	ESpellementType CurrentlySelectedElement = ESpellementType::Wind;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* Spring;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 120.f;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float MAX_MANA = 100;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Mana = MAX_MANA;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	ECastType CastType = ECastType::Projectile;
	
	APlayerController* PlayerController;

};
