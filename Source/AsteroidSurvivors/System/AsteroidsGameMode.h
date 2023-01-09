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
	// Getter & Setter
	TSubclassOf<UGameplayEffect> GetCollisionGameplayEffectClass() const;

	// Actions
	void GameOver() const;

public:
	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnGameOver OnGameOver;

protected:
	virtual void BeginPlay() override;

	// Spawn Handling
	UFUNCTION(BlueprintCallable)
	void SpawnAsteroid();
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

protected:
	// Basic Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnScreenMargin = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> CollisionGameplayEffectClass;

	// Asteroid Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldSpawnAsteroid = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AsteroidSpawnIntervalMin = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AsteroidSpawnIntervalMax = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAsteroid> AsteroidSpawnClass;

	// Enemy Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldSpawnEnemies = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemySpawnIntervalMin = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemySpawnIntervalMax = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAIShip> EnemySpawnClass;

private:
	FVector GetRandomSpawnLocation() const;

	FTimerHandle SpawnAsteroidTimer;
	FTimerHandle SpawnEnemyTimer;
};