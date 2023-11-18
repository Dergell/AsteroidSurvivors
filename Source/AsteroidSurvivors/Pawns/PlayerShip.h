// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "SpaceShip.h"
#include "PlayerShip.generated.h"

class ASpawnVolume;
class UCameraComponent;
class UInputComponent;
class USpringArmComponent;
struct FGameplayTag;
struct FInputActionValue;

/**
 * Class which implements the player ships. Extended by blueprints and classes.
 */
UCLASS()
class ASTEROIDSURVIVORS_API APlayerShip : public ASpaceShip
{
	GENERATED_BODY()

public:
	APlayerShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Get AbilitySystemComponent from player controller
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Get the camera location
	FVector GetCameraLocation() const;

protected:
	// Called when actor gets possessed by a controller
	virtual void PossessedBy(AController* NewController) override;

	// Player Input Handling
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Callback when an ability input tag is pressed
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);

	// Callback when an ability input tag is released
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	// Callback for movement input
	void Input_Move(const FInputActionValue& InputActionValue);

	// Callback for mouse aiming input
	void Input_AimMouse(const FInputActionValue& InputActionValue);

	// Callback for gamepad stick aiming input
	void Input_AimStick(const FInputActionValue& InputActionValue);

	// Callback when this ship overlaps another actor
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Callback when the player takes damage
	UFUNCTION()
	void OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	// SpringArm holding the camera
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	// The camera itself
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

private:
	// Determines camera shakes
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	// Class that will handle asteroid spawns around the player
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpawnVolume> SpawnVolumeClass;

	// Minimum length of spring arm
	UPROPERTY(EditAnywhere)
	float BoomMinLength = 2000.f;

	// Maximum length of spring arm
	UPROPERTY(EditAnywhere)
	float BoomMaxLength = 5000.f;

	// This will smooth out the extension and contraction of the spring arm
	UPROPERTY(EditAnywhere)
	float BoomSmoothingTime = 0.5f;
};