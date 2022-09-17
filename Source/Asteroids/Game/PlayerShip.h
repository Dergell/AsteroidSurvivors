// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "Asteroids/Interfaces/ProjectileInterface.h"
#include "Camera/CameraComponent.h"
#include "PlayerShip.generated.h"

class AItemProjectile;
class UCameraComponent;
class UInputComponent;
class USpringArmComponent;

/**
 * Class which implements the player ships. Extended by blueprints and classes.
 */
UCLASS()
class ASTEROIDS_API APlayerShip : public APawn, public IAbilitySystemInterface, public IProjectileInterface
{
	GENERATED_BODY()

public:
	APlayerShip();

	virtual void Tick(float DeltaSeconds) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintGetter)
	FVector GetCameraLocation() const
	{
		return Camera->GetComponentLocation();
	}

	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator,
		TSubclassOf<UGameplayEffect> ProjectileEffect) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	// Movement modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Acceleration = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedLimit = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollLimit = 30.f;

	// Camera modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomMinLength = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomMaxLength = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomSmoothingTime = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

private:
	void MoveHorizontal(float Value);
	void MoveVertical(float Value);
	void RotatePawn();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
};
