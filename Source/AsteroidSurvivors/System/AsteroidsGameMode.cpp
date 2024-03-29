// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidsGameMode.h"

#include "GameplayEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/AIShip.h"

AAsteroidsGameMode::AAsteroidsGameMode()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAsteroidsGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Schedule regular enemy spawns
	for (const FEnemySpawnConfig SpawnConfig : EnemySpawnConfig)
	{
		FTimerHandle SpawnEnemyTimer;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("SpawnEnemy"), SpawnConfig);
		GetWorldTimerManager().SetTimer(SpawnEnemyTimer, TimerDelegate, SpawnConfig.SecondsUntilFirstSpawn, false);
	}
}

void AAsteroidsGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TSubclassOf<UGameplayEffect> AAsteroidsGameMode::GetCollisionGameplayEffectClass() const
{
	if (UClass* Class = CollisionGameplayEffectClass.Get())
	{
		return Class;
	}

	return UGameplayEffect::StaticClass();
}

bool AAsteroidsGameMode::ShouldSpawnAsteroids() const
{
	return bShouldSpawnAsteroid;
}

float AAsteroidsGameMode::CalculateCollisionDamage(const float Impulse) const
{
	return FMath::RoundToZero(Impulse / CollisionDamageDivider);
}

void AAsteroidsGameMode::GameOver() const
{
	OnGameOver.Broadcast();
}

void AAsteroidsGameMode::SpawnEnemy(FEnemySpawnConfig SpawnConfig)
{
	if (!bShouldSpawnEnemies || EnemySpawnConfig.IsEmpty())
	{
		return;
	}

	// Spawn enemy
	const FVector Location = GetRandomSpawnLocation();
	const FRotator Rotation = GetRotatorTowardsPlayer(Location);
	GetWorld()->SpawnActor<AAIShip>(SpawnConfig.SpawnClass, Location, Rotation);

	// Reschedule next spawn
	FTimerHandle SpawnEnemyTimer;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("SpawnEnemy"), SpawnConfig);
	const float SpawnInterval = FMath::RandRange(SpawnConfig.SpawnIntervalMin, SpawnConfig.SpawnIntervalMax);
	GetWorldTimerManager().SetTimer(SpawnEnemyTimer, TimerDelegate, SpawnInterval, false);
}

FVector AAsteroidsGameMode::GetRandomSpawnLocation() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0));

	int32 SizeX, SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);

	// Trace from the camera edges to the games plane
	FVector TraceLocation, TraceDirection;
	PlayerController->DeprojectScreenPositionToWorld(0, 0, TraceLocation, TraceDirection);
	FVector UpperLeftCorner = FMath::LinePlaneIntersection(TraceLocation, TraceLocation + TraceDirection * 10000, FPlane(0, 0, 1, 0));

	PlayerController->DeprojectScreenPositionToWorld(SizeX, 0, TraceLocation, TraceDirection);
	FVector UpperRightCorner = FMath::LinePlaneIntersection(TraceLocation, TraceLocation + TraceDirection * 10000, FPlane(0, 0, 1, 0));

	PlayerController->DeprojectScreenPositionToWorld(SizeX, SizeY, TraceLocation, TraceDirection);
	FVector LowerRightCorner = FMath::LinePlaneIntersection(TraceLocation, TraceLocation + TraceDirection * 10000, FPlane(0, 0, 1, 0));

	PlayerController->DeprojectScreenPositionToWorld(0, SizeY, TraceLocation, TraceDirection);
	FVector LowerLeftCorner = FMath::LinePlaneIntersection(TraceLocation, TraceLocation + TraceDirection * 10000, FPlane(0, 0, 1, 0));

	// Add some safety margin
	UpperLeftCorner += FVector(SpawnScreenMargin, -SpawnScreenMargin, 0);
	UpperRightCorner += FVector(SpawnScreenMargin, SpawnScreenMargin, 0);
	LowerRightCorner += FVector(-SpawnScreenMargin, SpawnScreenMargin, 0);
	LowerLeftCorner += FVector(-SpawnScreenMargin, -SpawnScreenMargin, 0);

	// Get a random point on an random edge
	FVector Location;
	const int Edge = FMath::RandRange(1, 4);
	if (Edge == 1)
	{
		Location = FMath::Lerp(UpperLeftCorner, UpperRightCorner, FMath::FRand());
	}
	else if (Edge == 2)
	{
		Location = FMath::Lerp(UpperRightCorner, LowerRightCorner, FMath::FRand());
	}
	else if (Edge == 3)
	{
		Location = FMath::Lerp(LowerRightCorner, LowerLeftCorner, FMath::FRand());
	}
	else
	{
		Location = FMath::Lerp(LowerLeftCorner, UpperLeftCorner, FMath::FRand());
	}

	return Location;
}

FRotator AAsteroidsGameMode::GetRotatorTowardsPlayer(const FVector& SourceLocation) const
{
	const APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	const FVector Direction = Pawn->GetActorLocation() - SourceLocation;
	return Direction.Rotation();
}