/* Asteroid Survivors - Casual Rogue-Lite Indie Game
 * Copyright (C) 2022 Tony Schmich
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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

void AItemProjectile::ShootInDirection(const FVector& Direction)
{
    ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

void AItemProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Add event listeners
    CollisionComponent->OnComponentHit.AddDynamic(this, &AItemProjectile::OnComponentHit);
}

void AItemProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // If the component hit by this projectile is simulating physics, add an impact force
    if (OtherComp->IsSimulatingPhysics())
        OtherComp->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);

    // Tell the other actor it was hit
    IProjectileInterface* Interface = Cast<IProjectileInterface>(OtherActor);
    if (Interface)
        Interface->Execute_HitByProjectile(OtherActor);

    // Destroy the projectile
    Destroy();
}
