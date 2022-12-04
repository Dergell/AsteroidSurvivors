// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemWeightedSpawn.generated.h"

class AItemWeightedSpawn;

USTRUCT(BlueprintType)
struct FItemSpawnProbability
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMax = "100", UIMax = "100"))
	uint8 Probability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemWeightedSpawn> Item;
};

UCLASS()
class ASTEROIDS_API AItemWeightedSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemWeightedSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemSpawnProbability> SpawnProbability;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
