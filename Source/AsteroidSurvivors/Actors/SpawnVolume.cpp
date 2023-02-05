// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "Actors/SpawnVolume.h"

#include "Asteroid.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/AsteroidsGameMode.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Volume = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnVolume"));
	RootComponent = Volume;

	Volume->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AttachedToActor))
	{
		DistanceToAttachedActor = FVector::Distance(GetActorLocation(), AttachedToActor->GetActorLocation());
	}
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const int8 Count = GetMissingActorsCount();
	if (Count)
	{
		SpawnActors(Count);
	}

	if (bShouldMoveAroundAttachedActor)
	{
		ContinueMovement(DeltaTime);
	}
}

int8 ASpawnVolume::GetMissingActorsCount() const
{
	if (!SpawnActorClass)
	{
		return 0;
	}

	TArray<AActor*> ActorList;
	GetOverlappingActors(ActorList, SpawnActorClass);
	if (ActorList.Num() < MaxAllowedActors)
	{
		return MaxAllowedActors - ActorList.Num();
	}

	return 0;
}

void ASpawnVolume::SpawnActors(int8 Count) const
{
	const AAsteroidsGameMode* GameMode = Cast<AAsteroidsGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode && !GameMode->ShouldSpawnAsteroids() && SpawnActorClass->IsChildOf(AAsteroid::StaticClass()))
	{
		return;
	}

	while (Count)
	{
		const FVector2D Point = FMath::RandPointInCircle(Volume->GetScaledSphereRadius());
		const FVector Location = GetActorLocation() + FVector(Point.X, Point.Y, 0.f);
		GetWorld()->SpawnActor(SpawnActorClass, &Location);

		Count--;
	}
}

void ASpawnVolume::ContinueMovement(const float DeltaTime)
{
	if (!IsValid(AttachedToActor))
	{
		return;
	}

	FVector NewLocation = GetActorLocation() - AttachedToActor->GetActorLocation();
	NewLocation = NewLocation.RotateAngleAxis(DegreePerSecond * DeltaTime, FVector::UpVector);
	NewLocation = NewLocation.GetSafeNormal() * DistanceToAttachedActor;
	NewLocation += AttachedToActor->GetActorLocation();
	SetActorLocation(NewLocation);
}