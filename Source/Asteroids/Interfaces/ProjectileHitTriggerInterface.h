// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileHitTriggerInterface.generated.h"

class AItemProjectile;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UProjectileHitTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface which implements the main game mode framework. Extended by blueprints.
 */
class ASTEROIDS_API IProjectileHitTriggerInterface
{
	GENERATED_BODY()

public:

	/** Called when hit by a projectile */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger Reaction")
	void HitByProjectile();
};
