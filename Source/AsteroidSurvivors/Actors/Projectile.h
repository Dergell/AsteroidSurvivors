// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "Item.h"
#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

class UGameplayEffect;
class UProjectileMovementComponent;
class USphereComponent;

/**
 * Class which implements the projectile. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AProjectile : public AItem
{
	GENERATED_BODY()

public:
	AProjectile();

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
};