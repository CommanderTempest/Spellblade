// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpell.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"

AProjectileSpell::AProjectileSpell()
{
    PrimaryActorTick.bCanEverTick = true;
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;

    // SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
    // SphereCollider->SetupAttachment(ProjectileMesh);
}

void AProjectileSpell::BeginPlay()
{
    Super::BeginPlay();

    // when the mesh hits a component, go to the onhit function
    ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileSpell::OnHit);
}

void AProjectileSpell::FireSpell(UWorld* World, AActor* Owner, UClass* ProjectileClass, FVector SpawnLocation, FRotator SpawnRotation, ESpellementType Element)
{
    AProjectileSpell* Projectile = World->SpawnActor<AProjectileSpell>(ProjectileClass, SpawnLocation, SpawnRotation);
    Projectile->InitialLocation = SpawnLocation;
    Projectile->SetOwner(Owner);
    Projectile->SetSpellementType(Element);
}

void AProjectileSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    double distance = FVector::Dist(this->GetActorLocation(), InitialLocation);
    if (distance > ProjectileRange)
    {
        this->Destroy();
    }

}

void AProjectileSpell::OnHit(UPrimitiveComponent* HitComp, AActor* otherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Display, TEXT("Hit Object"));
    AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr) {
		Destroy();
		return;
	}
    
    if (otherActor && otherActor != this && otherActor != MyOwner)
    {
        if (otherActor->IsA(ASpellBase::StaticClass()))
        {
            ASpellBase* otherProjectile = Cast<ASpellBase>(otherActor);
            if (otherProjectile->GetIsHit()==false && this->GetIsHit()==false)
            {
                this->SetIsHit(true);
                
                ESpellementType SpellementType = otherProjectile->GetSpellementType();
                this->HandleCollision(SpellementType);
            }
        }
    }
    Destroy();
}