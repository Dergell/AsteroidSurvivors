// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ItemProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ASTEROIDS_API AItemProjectile : public AItemBase
{
	GENERATED_BODY()
	
public:

	AItemProjectile();

	/** The collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionComponent;

	/** The projecile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	/** Function that initializes the projectile's velocity in the shoot direction. */
	void ShootInDirection(const FVector& Direction);

protected:

	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	/** Called when the collision component hits something. */
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
