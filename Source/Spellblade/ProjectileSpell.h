// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellBase.h"
#include "ProjectileSpell.generated.h"


/**
 * Header file for all Projectile-Type spells in the game.
 * A projectile type spell is a spell that is launched towards a target
 * One example is like a fireball
 */


UCLASS()
class SPELLBLADE_API AProjectileSpell : public ASpellBase
{
	GENERATED_BODY()
	public:
		AProjectileSpell();
		virtual void Tick(float DeltaTime) override;
		static void FireSpell(UWorld* World, AActor* Owner, UClass* ProjectileClass, FVector SpawnLocation, FRotator SpawnRotation, ESpellementType Element);

	protected:
		virtual void BeginPlay() override;

		UPROPERTY(EditDefaultsOnly, Category = "Combat")
		float ProjectileSpeed = 3000.f;
		
		UPROPERTY(EditDefaultsOnly, Category = "Combat")
		float ProjectileRange = 6000.f;

		UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UStaticMeshComponent* ProjectileMesh;

		UPROPERTY(VisibleAnywhere, Category = "Movement")
		class UProjectileMovementComponent* ProjectileMovementComponent;

		UPROPERTY(EditAnywhere, Category = "Combat")
		float Damage = 50.f;

		// Initial spawn location, to be used for distance checking
		FVector InitialLocation;

		// UPROPERTY(EditDefaultsOnly, Category="Movement")
		// class USphereComponent* SphereCollider;

		UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* otherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
