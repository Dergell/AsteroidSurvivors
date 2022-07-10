/* Asteroid Survivors - Casual Rogue-Lite Indie Game
 * Copyright (C) 2022 Tony Schmich
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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
	
	// Called to initialize the projectile's velocity in a direction.
	void ShootInDirection(const FVector& Direction);

protected:

	// The collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asteroid Survivors")
	USphereComponent* CollisionComponent;

	// The projecile movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asteroid Survivors")
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

private:

	// Called when the collision component hits something.
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
