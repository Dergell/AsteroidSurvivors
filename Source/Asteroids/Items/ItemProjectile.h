// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "ItemBase.h"
#include "CoreMinimal.h"
#include "ItemProjectile.generated.h"

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
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    FVector NormalImpulse, const FHitResult& Hit);
};
