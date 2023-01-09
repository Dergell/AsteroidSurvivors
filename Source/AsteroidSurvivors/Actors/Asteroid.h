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
	AAsteroid();

	// Actions
	UFUNCTION(BlueprintCallable)
	void InitRandomMovement() const;

	// Interfaces
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator, FGameplayEffectSpecHandle EffectSpec) override;

protected:
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URotatingMovementComponent* RotatingMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* ExplosionComponent;

	// Members
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMesh*> AsteroidMeshes;

	// Settings
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InitVelocityMin = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InitVelocityMax = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InitRotationRateMin = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InitRotationRateMax = 25.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InitConeRadius = 55.f;

private:
	UFUNCTION()
	void OnExplosionFinished(UNiagaraComponent* PSystem);
};