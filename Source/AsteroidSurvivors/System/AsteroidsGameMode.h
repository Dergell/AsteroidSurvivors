// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AsteroidsGameMode.generated.h"

class UGameplayEffect;
class AAIShip;
class AAsteroid;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

/**
 * Class which implements the main game mode. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AAsteroidsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Get the CollisionGameplayEffectClass
	TSubclassOf<UGameplayEffect> GetCollisionGameplayEffectClass() const;

	// Calculate collision damage
	float CalculateCollisionDamage(float Impulse) const;

	// Broadcasts the GameOver event 
	void GameOver() const;

	// Delegate for the GameOver event
	UPROPERTY(BlueprintAssignable)
	FOnGameOver OnGameOver;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handles spawning of asteroids
	UFUNCTION(BlueprintCallable)
	void SpawnAsteroid();

	// Handles spawning of enemies
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

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

	// Minimum interval of asteroid spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AsteroidSpawnIntervalMin = 0.2f;

	// Maximum interval of asteroid spawns
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AsteroidSpawnIntervalMax = 2.f;

	// Asteroid class to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAsteroid> AsteroidSpawnClass;

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
	// Calculate a random spawn location just outside the viewport
	FVector GetRandomSpawnLocation() const;

	// Timer for asteroid spawns
	FTimerHandle SpawnAsteroidTimer;

	// Timer for enemy spawns
	FTimerHandle SpawnEnemyTimer;
};