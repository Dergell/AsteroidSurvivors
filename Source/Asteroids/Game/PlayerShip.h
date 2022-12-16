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
class UFloatingPawnMovement;
class UInputComponent;
class UInputConfig;
class USpringArmComponent;
struct FGameplayTag;
struct FInputActionValue;

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
	virtual FVector GetVelocity() const override;

	// Getter & Setter
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintGetter)
	FVector GetCameraLocation() const { return Camera->GetComponentLocation(); }

	// Actions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();

	// Interfaces
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator, TSubclassOf<UGameplayEffect> ProjectileEffect) override;

public:
	// Input Handling
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputConfig* InputConfig;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	// Input Handling
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_AimMouse(const FInputActionValue& InputActionValue);
	void Input_AimStick(const FInputActionValue& InputActionValue);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFloatingPawnMovement* MovementComponent;

	// Movement Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollLimit = 30.f;

	// Camera Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomMinLength = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomMaxLength = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoomSmoothingTime = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

private:
	void RotatePawn();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};