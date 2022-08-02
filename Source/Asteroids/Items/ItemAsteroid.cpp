// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "ItemAsteroid.h"

#include "AI/NavigationSystemBase.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AItemAsteroid::AItemAsteroid()
{
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	ExplosionComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
	ExplosionComponent->SetupAttachment(Mesh);
	ExplosionComponent->SetAutoActivate(false);
	ExplosionComponent->SetTemplate(ExplosionParticleSystem);
}

void AItemAsteroid::InitRandomMovement() const
{
	const AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Aim in the movement direction of the player, then randomize in cone
	FVector Direction = PlayerPawn->GetActorLocation() + PlayerPawn->GetVelocity() - GetActorLocation();
	Direction = FMath::VRandCone(Direction, FMath::DegreesToRadians(InitConeRadius));

	Mesh->AddImpulse(Direction * FMath::RandRange(InitVelocityMin, InitVelocityMax), NAME_None, true);
	RotatingMovement->RotationRate = FRotator(FMath::RandRange(InitRotationRateMin, InitRotationRateMax),
	                                          FMath::RandRange(InitRotationRateMin, InitRotationRateMax),
	                                          FMath::RandRange(InitRotationRateMin, InitRotationRateMax));
}

void AItemAsteroid::BeginPlay()
{
	Super::BeginPlay();

	ExplosionComponent->OnSystemFinished.AddDynamic(this, &AItemAsteroid::OnExplosionFinished);
}

void AItemAsteroid::PostActorCreated()
{
	const int32 RandomIndex = FMath::RandRange(1, AsteroidMeshes.Num()) - 1;
	Mesh->SetStaticMesh(AsteroidMeshes[RandomIndex]);
}

void AItemAsteroid::OnExplosionFinished(UParticleSystemComponent* FinishedComponent)
{
	Destroy();
}

void AItemAsteroid::HitByProjectile_Implementation(APawn* ProjectileInstigator)
{
	APlayerState* State = ProjectileInstigator->GetPlayerState();

	if (const IItemInterface* Interface = Cast<IItemInterface>(State))
	{
		Interface->Execute_UpdateScore(State, PointsValue);
	}

	RotatingMovement->RotationRate = FRotator::ZeroRotator;
	Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetVisibility(false);

	ExplosionComponent->Activate();
}
