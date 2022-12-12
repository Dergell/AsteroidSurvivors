// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "ItemBase.h"
#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ItemProjectile.generated.h"

class UGameplayEffect;
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

	// Getter & Setter
	FORCEINLINE float GetInitialSpeed() const { return ProjectileMovementComponent->InitialSpeed; }

protected:
	virtual void BeginPlay() override;

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Gameplay Ability System
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffect;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};