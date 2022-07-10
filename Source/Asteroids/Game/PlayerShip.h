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

class AItemProjectile;
class UCameraComponent;
class UInputComponent;
class USpringArmComponent;

/**
 * Class which implements the player ships. Extended by blueprints and classes.
 */
UCLASS()
class ASTEROIDS_API APlayerShip : public APawn
{
	GENERATED_BODY()

public:

	APlayerShip();

protected:

	// Mesh of the player ship
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asteroid Survivors")
	UStaticMeshComponent* Mesh;

	// SpringArm for the camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asteroid Survivors")
	USpringArmComponent* SpringArm;

	// Camera of the player ship
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asteroid Survivors")
	UCameraComponent* Camera;

	// How much force should be applied for movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Survivors")
	float MoveForce = 500.f;

	// How fast should it spin around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Survivors")
	float TurnSpeed = 1.f;

	// Speed of the roll while turning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Survivors")
	float RollSpeed = 1.f;

	// How many degrees the roll while turning should do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Survivors")
	float RollLimit = 30.f;

	// Which projectile class this ship will use
	UPROPERTY(EditDefaultsOnly, Category = "Asteroid Survivors")
	TSubclassOf<AItemProjectile> ProjectileClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	
	// Add lateral force
	void MoveRight(float Value);

	// Add longitudinal force
	void MoveForward(float Value);

	// Rotate around yaw axis
	void TurnRight(float Value);

	// Shoot all manual weapons
	void Shoot();

	// Process overlap
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Process collisions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
