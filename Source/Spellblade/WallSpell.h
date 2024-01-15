// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellBase.h"
#include "WallSpell.generated.h"

/**
 * Header file for Wall-like spells in the game
 * A wall spell is where you left-click somewhere in range
 * and a wall of your selected element spawns there
 */
UCLASS()
class SPELLBLADE_API AWallSpell : public ASpellBase
{
	GENERATED_BODY()
	AWallSpell();
	
	public:
		virtual void Tick(float DeltaTime) override;
		static void FireSpell(UWorld* World, AActor* Owner, UClass* WallClass, FVector SpawnLocation, FRotator SpawnRotation, ESpellementType Element);

	protected:
		UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UStaticMeshComponent* WallMesh;

		UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* otherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	private:
		// How long in seconds the wall will last for
		float LifeDuration = 20;

		void WallProjectileCollision(AActor* otherActor);
	
};
