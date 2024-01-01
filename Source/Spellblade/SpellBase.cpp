// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellBase.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpellBase::ASpellBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpellBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpellBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpellBase::FireSpell()
{
	// take mana and subtract or sumthin I guess?
}


void ASpellBase::HandleCollision(ESpellementType otherActorElement)
{
	// in future, may want to make a different collision function for each element; for readability
	switch (this->GetSpellementType())
	{
		case (ESpellementType::Wind):
			UE_LOG(LogTemp, Display, TEXT("Hit Wind Object"));
			HandleWindCollision(otherActorElement);
			break;
		case (ESpellementType::Fire):
			UE_LOG(LogTemp, Display, TEXT("Hit Fire Object"));
			if (otherActorElement == ESpellementType::Water) {HandleWaterFireCollision();}
			break;
		case (ESpellementType::Water):
			UE_LOG(LogTemp, Display, TEXT("Hit Water Object"));
			if (otherActorElement == ESpellementType::Fire) {HandleWaterFireCollision();}
			break;
		case (ESpellementType::Earth):
			UE_LOG(LogTemp, Display, TEXT("Hit Earth Object"));
			break;
		default:
			UE_LOG(LogTemp, Display, TEXT("Spell type not set"));
	}
}
void ASpellBase::HandleWindCollision(ESpellementType otherActorElement)
{
	//
}

void ASpellBase::HandleWaterFireCollision()
{
	// Produce steam at collision location
	if (WaterFireParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, WaterFireParticles, GetActorLocation(), GetActorRotation());
	}
	if (SteamSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SteamSound, GetActorLocation(), GetActorRotation());
	}
}

