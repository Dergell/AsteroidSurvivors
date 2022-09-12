// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "ItemProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AItemProjectile::AItemProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a sphere as a simple collision representation.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

	// Attach mesh
	Mesh->SetupAttachment(CollisionComponent);
}

void AItemProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Add event listeners
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemProjectile::OnComponentBeginOverlap);
}

void AItemProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                              const FHitResult& SweepResult)
{
	// Ignore the actor that shot this projectile
	if (Owner == OtherActor)
		return;

	if (OtherActor != nullptr)
	{
		// Tell the other actor it was hit
		IProjectileInterface* Interface = Cast<IProjectileInterface>(OtherActor);
		if (Interface)
			Interface->Execute_HitByProjectile(OtherActor, GetInstigator(), GameplayEffect);
	}

	// Destroy the projectile
	Destroy();
}
