// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "Asteroid.h"

#include "Actors/ItemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameplayEffectTypes.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

AAsteroid::AAsteroid()
{
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	ExplosionComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ExplosionEffect"));
	ExplosionComponent->SetupAttachment(Mesh);
}

void AAsteroid::InitRandomMovement() const
{
	const AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Aim in the movement direction of the player, then randomize in cone
	FVector Direction = PlayerPawn->GetActorLocation() + PlayerPawn->GetVelocity() - GetActorLocation();
	Direction = FMath::VRandCone(Direction, FMath::DegreesToRadians(InitConeRadius));

	Mesh->AddImpulse(Direction * FMath::RandRange(InitVelocityMin, InitVelocityMax), NAME_None, true);
	RotatingMovement->RotationRate = FRotator(FMath::RandRange(InitRotationRateMin, InitRotationRateMax), FMath::RandRange(InitRotationRateMin, InitRotationRateMax), FMath::RandRange(InitRotationRateMin, InitRotationRateMax));
}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();

	ExplosionComponent->OnSystemFinished.AddDynamic(this, &AAsteroid::OnExplosionFinished);
}

void AAsteroid::PostActorCreated()
{
	const int32 RandomIndex = FMath::RandRange(1, AsteroidMeshes.Num()) - 1;

	if (AsteroidMeshes.IsValidIndex(RandomIndex))
	{
		Mesh->SetStaticMesh(AsteroidMeshes[RandomIndex]);
	}
}

void AAsteroid::OnExplosionFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}

void AAsteroid::HitByProjectile_Implementation(APawn* ProjectileInstigator, FGameplayEffectSpecHandle EffectSpec)
{
	if (IsValid(ProjectileInstigator))
	{
		APlayerState* State = ProjectileInstigator->GetPlayerState();
		if (const IItemInterface* Interface = Cast<IItemInterface>(State))
		{
			Interface->Execute_UpdateScore(State, PointsValue);
		}
	}

	RotatingMovement->RotationRate = FRotator::ZeroRotator;
	Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetVisibility(false);

	ExplosionComponent->ActivateSystem();
}