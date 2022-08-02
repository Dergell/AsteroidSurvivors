// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "GameModeMain.h"

#include "Asteroids/Items/ItemAsteroid.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AGameModeMain::BeginPlay()
{
	Super::BeginPlay();

	// Schedule regular asteroid spawns
	const float SpawnInterval = FMath::RandRange(AsteroidSpawnIntervalMin, AsteroidSpawnIntervalMax);
	GetWorldTimerManager().SetTimer(SpawnAsteroidTimer, this, &AGameModeMain::SpawnAsteroid, SpawnInterval);
}

void AGameModeMain::SpawnAsteroid()
{
	if (ShouldSpawnAsteroid)
	{
		const APlayerController* PlayerController = Cast<APlayerController>(
			UGameplayStatics::GetPlayerController(GetWorld(), 0));

		int32 SizeX, SizeY;
		PlayerController->GetViewportSize(SizeX, SizeY);

		// Trace from the camera edges to the games plane
		FVector TraceLocation, TraceDirection;
		PlayerController->DeprojectScreenPositionToWorld(0, 0, TraceLocation, TraceDirection);
		FVector UpperLeftCorner = FMath::LinePlaneIntersection(TraceLocation,
		                                                       TraceLocation + TraceDirection * 10000,
		                                                       FPlane(0, 0, 1, 0));

		PlayerController->DeprojectScreenPositionToWorld(SizeX, 0, TraceLocation, TraceDirection);
		FVector UpperRightCorner = FMath::LinePlaneIntersection(TraceLocation,
		                                                        TraceLocation + TraceDirection * 10000,
		                                                        FPlane(0, 0, 1, 0));

		PlayerController->DeprojectScreenPositionToWorld(SizeX, SizeY, TraceLocation, TraceDirection);
		FVector LowerRightCorner = FMath::LinePlaneIntersection(TraceLocation,
		                                                        TraceLocation + TraceDirection * 10000,
		                                                        FPlane(0, 0, 1, 0));

		PlayerController->DeprojectScreenPositionToWorld(0, SizeY, TraceLocation, TraceDirection);
		FVector LowerLeftCorner = FMath::LinePlaneIntersection(TraceLocation,
		                                                       TraceLocation + TraceDirection * 10000,
		                                                       FPlane(0, 0, 1, 0));

		// Add some safety margin
		UpperLeftCorner += FVector(100, -100, 0);
		UpperRightCorner += FVector(100, 100, 0);
		LowerRightCorner += FVector(-100, 100, 0);
		LowerLeftCorner += FVector(-100, -100, 0);

		// Get a random point on an random edge
		FVector Location;
		const int Edge = FMath::RandRange(1, 4);
		if (Edge == 1)
			Location = FMath::Lerp(UpperLeftCorner, UpperRightCorner, FMath::FRand());
		else if (Edge == 2)
			Location = FMath::Lerp(UpperRightCorner, LowerRightCorner, FMath::FRand());
		else if (Edge == 3)
			Location = FMath::Lerp(LowerRightCorner, LowerLeftCorner, FMath::FRand());
		else
			Location = FMath::Lerp(LowerLeftCorner, UpperLeftCorner, FMath::FRand());

		// Get a random Rotation and Scale
		const FRotator Rotation = UKismetMathLibrary::RandomRotator(true);
		const FVector Scale = FVector(FMath::RandRange(0.8f, 2.5f));

		// Spawn the asteroid and init random movement
		const FTransform* Transform = new FTransform(Rotation, Location, Scale);
		if (const AItemAsteroid* Asteroid = Cast<AItemAsteroid>(GetWorld()->SpawnActor(AsteroidSpawnClass, Transform)))
			Asteroid->InitRandomMovement();
	}

	// Reschedule next spawn
	const float SpawnInterval = FMath::RandRange(AsteroidSpawnIntervalMin, AsteroidSpawnIntervalMax);
	GetWorldTimerManager().SetTimer(SpawnAsteroidTimer, this, &AGameModeMain::SpawnAsteroid, SpawnInterval);
}
