// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellementType.h"
#include "SpellBase.generated.h"


/*
	Base Header File for all spells in the game
*/

UCLASS()
class SPELLBLADE_API ASpellBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellBase();
	ESpellementType GetSpellementType() const {return Element;}
	void SetSpellementType(ESpellementType SpellType) {this->Element=SpellType;}

	// This debounce prevents a spell's effect from triggering twice for both spells
	bool GetIsHit() const {return IsHit;}
	void SetIsHit(bool value) {IsHit = value;}

	void FireSpell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float CastTime;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ManaCost;

	UPROPERTY(EditAnywhere, Category = "Combat")
	ESpellementType Element;

	void HandleCollision(ESpellementType otherActorElement);

private:
	bool IsHit = false;

	UPROPERTY(EditAnywhere, Category = "Collision Effects")
	class UParticleSystem* WaterFireParticles;

	UPROPERTY(EditAnywhere, Category = "Collision Effects")
	class USoundBase* SteamSound;

	void HandleWindCollision(ESpellementType otherActorElement);
	void HandleWaterFireCollision();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};