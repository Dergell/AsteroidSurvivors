// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemAsteroid.generated.h"

class URotatingMovementComponent;
class UNiagaraComponent;

/**
 * Class which implements the asteroid actors. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API AItemAsteroid : public AItemBase
{
	GENERATED_BODY()

public:
	AItemAsteroid();

	// Actions
	UFUNCTION(BlueprintCallable)
	void InitRandomMovement() const;

	// Interfaces
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator, TSubclassOf<UGameplayEffect> ProjectileEffect) override;

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