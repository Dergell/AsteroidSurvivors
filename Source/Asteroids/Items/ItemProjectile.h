// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "ItemBase.h"
#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ItemProjectile.generated.h"

class UGameplayEffect;
class USphereComponent;
class UProjectileMovementComponent;

/**
 * Class which implements the projectile. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API AItemProjectile : public AItemBase
{
	GENERATED_BODY()

public:
	AItemProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Get the value of InitialSpeed
	float GetInitialSpeed() const { return ProjectileMovementComponent->InitialSpeed; }

private:
	// Called on overlap with other actors
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// SphereComponent used for collision detection
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	// MovementComponent of this projectile
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// GameplayEffect applied by this projectile
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	// The amount should be applied by the GameplayEffect (ex. damage on a damage effect)
	UPROPERTY(EditAnywhere)
	float EffectAmount;
};