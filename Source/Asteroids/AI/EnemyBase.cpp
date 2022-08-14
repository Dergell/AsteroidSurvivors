// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "EnemyBase.h"

#include "Asteroids/Components/AIMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);

	MovementComp = CreateDefaultSubobject<UAIMovementComponent>(TEXT("MovementComp"));
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FaceTargetDirection(DeltaTime);
}

void AEnemyBase::HitByProjectile_Implementation(APawn* ProjectileInstigator)
{
	Destroy();
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyBase::FaceTargetDirection(float DeltaTime)
{
	const FRotator CurrentRotation = GetActorRotation();
	const FVector TargetVector = Target->GetTargetLocation(this) - GetActorLocation();
	FRotator TargetRotation = TargetVector.Rotation();

	// Add some roll to the turn
	TargetRotation.Roll = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);

	// For the actual turn, just add the direction to the existing yaw
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TurnSpeed);
	// Clamp roll so we don't overshoot
	NewRotation.Roll = FMath::Clamp(NewRotation.Roll, -RollLimit, RollLimit);
	// Cancel any pitch by physics
	NewRotation.Pitch = 0;

	// Normalize and set to Mesh
	SetActorRotation(NewRotation);
}
