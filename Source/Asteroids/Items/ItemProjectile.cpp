// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemProjectile.h"

AItemProjectile::AItemProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Use a sphere as a simple collision representation.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(15.0f);
    RootComponent = CollisionComponent;

    // Use this component to drive this projectile's movement.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

    // Attach mesh
    Mesh->SetupAttachment(CollisionComponent);
}

void AItemProjectile::BeginPlay()
{
    Super::BeginPlay();

    CollisionComponent->OnComponentHit.AddDynamic(this, &AItemProjectile::OnComponentHit);
}

void AItemProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // If the component hit by this projectile is simulating physics, add an impact force
    if (OtherComp->IsSimulatingPhysics())
        OtherComp->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);

    // Execute the HitByProjectile on the actor
    if (OtherActor->Implements<UProjectileHitTriggerInterface>()) {
        IProjectileHitTriggerInterface* Interface = Cast<IProjectileHitTriggerInterface>(OtherActor);
        Interface->Execute_HitByProjectile(OtherActor);
    }

    // Destroy the projectile
    Destroy();
}

void AItemProjectile::ShootInDirection(const FVector& Direction)
{
    ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}
