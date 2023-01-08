// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "ItemProjectile.h"

#include "AbilitySystemComponent.h"
#include "AsteroidsGameplayTags.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "Game/SpaceShip.h"
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

void AItemProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Ignore the actor that shot this projectile
	if (OtherActor == nullptr || OtherActor == Owner)
	{
		return;
	}

	const ASpaceShip* Ship = Cast<ASpaceShip>(OtherActor);
	const IProjectileInterface* Interface = Cast<IProjectileInterface>(OtherActor);

	// If OtherActor is a SpaceShip, we know we have an ASC and can add the gameplay effects
	if (Ship)
	{
		const UAbilitySystemComponent* ASC = Ship->GetAbilitySystemComponent();

		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		EffectContext.AddInstigator(GetInstigator(), GetInstigator());
		EffectContext.AddHitResult(SweepResult);

		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FAsteroidsGameplayTags::Get().Effect_Damage, EffectAmount);

		Ship->Execute_HitByProjectile(OtherActor, GetInstigator(), SpecHandle);
	}
	// If OtherActor is NOT a SpaceShip, but implements the ProjectileInterface, we notify it without gameplay effects
	else if (Interface)
	{
		Interface->Execute_HitByProjectile(OtherActor, GetInstigator(), FGameplayEffectSpecHandle());
	}

	// Destroy the projectile
	Destroy();
}