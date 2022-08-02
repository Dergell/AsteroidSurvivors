// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemAsteroid.generated.h"

class URotatingMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;

/**
 * Class which implements the asteroid actors. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API AItemAsteroid : public AItemBase
{
	GENERATED_BODY()

public:
	AItemAsteroid();

	UFUNCTION(BlueprintCallable)
	void InitRandomMovement() const;

	// Called when hit by a projectile
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void PostActorCreated() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URotatingMovementComponent* RotatingMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ExplosionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMesh*> AsteroidMeshes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* ExplosionParticleSystem;

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
	void OnExplosionFinished(UParticleSystemComponent* FinishedComponent);
};
