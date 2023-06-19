// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AsteroidsGameMode.generated.h"

class UGameplayEffect;
class AAIShip;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

USTRUCT(BlueprintType)
struct FEnemySpawnConfig
{
	GENERATED_BODY()

	// Start spawning after this much time has passed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondsUntilFirstSpawn = 0.f;

	// Minimum interval between spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnIntervalMin = 1.f;

	// Maximum interval between spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnIntervalMax = 2.f;

	// Enemy class to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAIShip> SpawnClass;
};

/**
 * Class which implements the main game mode. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AAsteroidsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAsteroidsGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get the CollisionGameplayEffectClass
	TSubclassOf<UGameplayEffect> GetCollisionGameplayEffectClass() const;

	// Get bShouldSpawnAsteroid
	bool ShouldSpawnAsteroids() const;

	// Calculate collision damage
	float CalculateCollisionDamage(float Impulse) const;

	// Broadcasts the GameOver event 
	void GameOver() const;

	// Delegate for the GameOver event
	UPROPERTY(BlueprintAssignable)
	FOnGameOver OnGameOver;

protected:
	// Handles spawning of enemies
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy(FEnemySpawnConfig SpawnConfig);

private:
	// Calculate a random spawn location just outside the viewport
	FVector GetRandomSpawnLocation() const;

	// Returns rotator from SourceLocation towards player
	FRotator GetRotatorTowardsPlayer(const FVector& SourceLocation) const;

protected:
	// Additional screen margin to prevent spawns at the edge of the viewport
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnScreenMargin = 200.f;

	// Class of the gameplay effect for collisions
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> CollisionGameplayEffectClass;

	// Used to adjust the collision damage. Higher value = less damage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CollisionDamageDivider = 100000.f;

	// Enable to spawn asteroids
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldSpawnAsteroid = false;

	// Enable to spawn enemies
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldSpawnEnemies = false;

	// Configuration for enemy spawns
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FEnemySpawnConfig> EnemySpawnConfig;
};