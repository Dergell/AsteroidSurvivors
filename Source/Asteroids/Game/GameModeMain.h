// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeMain.generated.h"

class UGameplayEffect;
class AEnemyBase;
class AItemAsteroid;

/**
 * Class which implements the main game mode. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API AGameModeMain : public AGameModeBase
{
	GENERATED_BODY()

public:
	TSubclassOf<UGameplayEffect> GetCollisionGameplayEffectClass() const;
	float GetCollisionThreshold() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SpawnAsteroid();
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnScreenMargin = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> CollisionGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CollisionThreshold = 50000.f;

	// Asteroid Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldSpawnAsteroid = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AsteroidSpawnIntervalMin = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AsteroidSpawnIntervalMax = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AItemAsteroid> AsteroidSpawnClass;

	// Enemy Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldSpawnEnemies = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemySpawnIntervalMin = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemySpawnIntervalMax = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AEnemyBase> EnemySpawnClass;

private:
	FVector GetRandomSpawnLocation() const;

	FTimerHandle SpawnAsteroidTimer;
	FTimerHandle SpawnEnemyTimer;
};
