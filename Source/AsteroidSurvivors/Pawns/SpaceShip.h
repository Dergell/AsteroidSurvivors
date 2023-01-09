// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Actors/ProjectileInterface.h"
#include "GameFramework/Pawn.h"
#include "SpaceShip.generated.h"

class UAsteroidsMovementComponent;
class UCapsuleComponent;
class UNiagaraSystem;
class USoundBase;

/**
 * Base class for all spaceship pawns. Extended by blueprints and classes.
 */
UCLASS(abstract)
class ASTEROIDSURVIVORS_API ASpaceShip : public APawn, public IAbilitySystemInterface, public IProjectileInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get AbilitySystemComponent (nullptr, must be overriden in subclasses!)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Callback used when hit by a projectile
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator, FGameplayEffectSpecHandle EffectSpec) override;

	// Trigger explosion
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();

protected:
	// Callback when this ship collides with another actor
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// RootComponent used for movement and collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;

	// Static mesh of the spaceship
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	// Custom physics based movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAsteroidsMovementComponent* MovementComponent;

private:
	// The explosion particle effect to be spawned on death 
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* ExplosionParticleEffect;

	// The explosion sound effect to be spawned on death
	UPROPERTY(EditDefaultsOnly)
	USoundBase* ExplosionSoundEffect;
};