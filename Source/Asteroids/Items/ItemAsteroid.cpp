// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "ItemAsteroid.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "GameFramework/PlayerState.h"

void AItemAsteroid::BeginPlay()
{
	Super::BeginPlay();
}

void AItemAsteroid::PostActorCreated()
{
	int32 RandomIndex = FMath::RandRange(1, AsteroidMeshes.Num()) - 1;
	Mesh->SetStaticMesh(AsteroidMeshes[RandomIndex]);
}

void AItemAsteroid::HitByProjectile_Implementation(APawn* ProjectileInstigator)
{
	APlayerState* State = ProjectileInstigator->GetPlayerState();

	IItemInterface* Interface = Cast<IItemInterface>(State);
	if (Interface)
	{
		Interface->Execute_UpdateScore(State, PointsValue);
	}

	Destroy();
}
