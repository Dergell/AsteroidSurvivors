// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "ItemWeightedSpawn.h"

// Sets default values
AItemWeightedSpawn::AItemWeightedSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AItemWeightedSpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemWeightedSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

