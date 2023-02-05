// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Asteroid.generated.h"

class UNiagaraComponent;
class URotatingMovementComponent;

/**
 * Class which implements the asteroid actors. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AAsteroid : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAsteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// This is called before calling construction scripts, but after native components have been created
	virtual void PostActorCreated() override;

public:
	// Callback used when hit by a projectile
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator, FGameplayEffectSpecHandle EffectSpec) override;

protected:
	// Will apply an impulse in a random direction and add random rotation
	UFUNCTION(BlueprintCallable)
	void InitRandomMovement() const;

protected:
	// Component which controls rotation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URotatingMovementComponent* RotatingMovement;

	// VFX to play on destruction of the asteroid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* ExplosionComponent;

	// On spawn one of these meshes is randomly assigned to the asteroid
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMesh*> AsteroidMeshes;

	// Minimum scale for spawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SpawnScaleMin = 0.8f;

	// Maximum scale for spawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SpawnScaleMax = 2.5f;
	
	// Minimum velocity for InitRandomMovement
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InitVelocityMin = 0.f;

	// Maximum velocity for InitRandomMovement
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InitVelocityMax = 800.f;

	// Minimum rotation rate for InitRandomMovement
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InitRotationRateMin = 0.f;

	// Maximum rotation rate for InitRandomMovement
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InitRotationRateMax = 25.f;

private:
	// Callback to destroy the object once the VFX is finished
	UFUNCTION()
	void OnExplosionFinished(UNiagaraComponent* PSystem);
};