// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpell.h"

#include "ProjectileSpell.h"
#include "Elements/Framework/TypedElementOwnerStore.h"
#include "GameFramework/Actor.h"

AWallSpell::AWallSpell()
{
	PrimaryActorTick.bCanEverTick = false;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Mesh"));
	RootComponent = WallMesh;
}

void AWallSpell::Tick(float DeltaTime)
{
	
}

void AWallSpell::FireSpell(UWorld* World, AActor* Owner, UClass* WallClass, FVector SpawnLocation, FRotator SpawnRotation, ESpellementType Element)
{
	AWallSpell* Wall = World->SpawnActor<AWallSpell>(WallClass, SpawnLocation, SpawnRotation);
	// TODO: IF WIND, CAN SPAWN IN MID-AIR
	Wall->SetOwner(Owner);
	Wall->SetSpellementType(Element);
}

void AWallSpell::OnHit(UPrimitiveComponent* HitComp, AActor* otherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("Hit Object"));
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr) {
		Destroy();
		return;
	}

	if (otherActor && otherActor != this && otherActor != MyOwner)
	{
		if (otherActor->IsA(AProjectileSpell::StaticClass()))
		{
			WallProjectileCollision(otherActor);
		}
		else if (otherActor->IsA(AWallSpell::StaticClass()))
		{
			// Wall to Wall Collision
		}
	}
	Destroy();
}

void AWallSpell::WallProjectileCollision(AActor* otherActor)
{
	ASpellBase* otherProjectile = Cast<AProjectileSpell>(otherActor);
	if (otherProjectile->GetIsHit()==false && this->GetIsHit()==false)
	{
		this->SetIsHit(true);
                
		ESpellementType SpellementType = otherProjectile->GetSpellementType();
		//this->HandleCollision(SpellementType);
	}
}
