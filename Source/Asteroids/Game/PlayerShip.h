// Fill out your copyright notice in the Description page of Project Settings.

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
