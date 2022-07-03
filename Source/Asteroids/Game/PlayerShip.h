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

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerShip.generated.h"

/**
 * Class which implements the player ships. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API APlayerShip : public APawn
{
	GENERATED_BODY()

public:

	APlayerShip();

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Add lateral force */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	/** Add lateral force */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	/** Add lateral force */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* Camera;

	/** Add lateral force */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveForce = 500.f;

	/** Add lateral force */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnSpeed = 1.f;

	/** Add lateral force */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollSpeed = 1.f;

	/** Add lateral force */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollLimit = 30.f;

	/** Add lateral force */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AItemProjectile> ProjectileClass;

public:

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	/** Add lateral force */
	void MoveRight(float Value);

	/** Add longitudinal force */
	void MoveForward(float Value);

	/** Process turn rotation */
	void TurnRight(float Value);

	/** Shoot all manual weapons */
	void Shoot();

	/** Process collisions */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
