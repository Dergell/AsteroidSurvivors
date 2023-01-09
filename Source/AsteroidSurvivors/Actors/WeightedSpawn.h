// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeightedSpawn.generated.h"

class AItem;

USTRUCT(BlueprintType)
struct FItemSpawnProbability
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMax = "100", UIMax = "100"))
	uint8 Probability = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;
};

UCLASS()
class ASTEROIDSURVIVORS_API AWeightedSpawn : public AActor
{
	GENERATED_BODY()

public:
	AWeightedSpawn();

protected:
	virtual void BeginPlay() override;

protected:
	// Components
	UPROPERTY(VisibleInstanceOnly)
	UChildActorComponent* ChildActor;

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemSpawnProbability> SpawnProbability;
};