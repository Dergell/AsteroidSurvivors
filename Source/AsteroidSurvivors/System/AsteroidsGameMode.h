// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AsteroidsGameMode.generated.h"

class UGameplayEffect;
class AAIShip;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

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
	void SpawnEnemy();

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

	// Minimum interval of enemy spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemySpawnIntervalMin = 1.f;

	// Maximum interval of enemy spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemySpawnIntervalMax = 2.f;

	// Enemy class to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAIShip> EnemySpawnClass;

private:
	// Timer for enemy spawns
	FTimerHandle SpawnEnemyTimer;
};