// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

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

	virtual void Tick(float DeltaSeconds) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	// Movement modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveForce = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollLimit = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)

	// Camera modifiers
	float BoomMinLength = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomMaxLength = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomSmoothingTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomExtensionSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomExtensionPower = 1.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItemProjectile> ProjectileClass;

private:
	void MoveRight(float Value);
	void MoveForward(float Value);
	void TurnRight(float Value);
	void Shoot();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse, const FHitResult& Hit);
};
