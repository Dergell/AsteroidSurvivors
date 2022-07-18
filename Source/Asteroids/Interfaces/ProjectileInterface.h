// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UProjectileInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Framework for projectile interactions. Extended by blueprints.
 */
class ASTEROIDS_API IProjectileInterface
{
	GENERATED_BODY()

public:
	// Called when hit by a projectile
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HitByProjectile(APawn* ProjectileInstigator);
};
