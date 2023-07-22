// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "WeightedSpawn.h"

// Sets default values
AWeightedSpawn::AWeightedSpawn()
{
	PrimaryActorTick.bCanEverTick = false;

	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	RootComponent = ChildActor;
}

// Called when the game starts or when spawned
void AWeightedSpawn::BeginPlay()
{
	Super::BeginPlay();

	uint32 ProbabilityTotal = 0;
	for (const FItemSpawnProbability Element : SpawnProbability)
	{
		ProbabilityTotal += Element.Probability;
	}

	uint8 RandomValue = FMath::RandRange(0, ProbabilityTotal - 1);
	for (const FItemSpawnProbability Element : SpawnProbability)
	{
		if (RandomValue < Element.Probability)
		{
			ChildActor->SetChildActorClass(Element.ItemClass);
			break;
		}

		RandomValue -= Element.Probability;
	}

	if (!IsValid(ChildActor->GetChildActorClass()))
	{
		Destroy();
	}
}