// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "SpellementType.h"
#include "BasePawn.generated.h"

/*
This is the character header file
*/

class UInputMappingContext;
class UInputAction;

UCLASS()
class SPELLBLADE_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// *** INPUTS ***

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SpellementSelectAction;

	void Fire(); 
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Turn(float Value);
	void SelectSpellement(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	APlayerController* GetPlayerController() const {return PlayerController;}

private:
	// UPROPERTY(EditAnywhere, Category="Input")
	// TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ASpellBase> ProjectileClass;

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

	// Debounce for fire
	bool IsFiring = false;

	APlayerController* PlayerController;

};
