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
	Wall->SetOwner(Owner);
	Wall->SetSpellementType(Element);
	
	// if not wind/none, the wall should be on the ground
	if (Element != ESpellementType::Wind && Element != ESpellementType::None)
	{
		FHitResult OutHit;
		FVector StartLoc = Wall->GetActorLocation();
		FVector EndLoc = (-(Wall->GetActorUpVector() * 500.f) + StartLoc);
		
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(Wall); //ignore self
		
		DrawDebugLine(World->GetWorld(), StartLoc, EndLoc, FColor::Green, false, 1000, 0, 5);
		bool isHit = World->LineTraceSingleByChannel(OutHit, StartLoc, EndLoc, ECC_WorldStatic, CollisionQueryParams);
		if (isHit)
		{
			UE_LOG(LogTemp, Display, TEXT("Hit object %s"), *OutHit.GetComponent()->GetName());
			Wall->SetActorLocation(OutHit.ImpactPoint);
		}
	}
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
