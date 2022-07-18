// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemAsteroid.generated.h"

/**
 * Class which implements the asteroid actors. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API AItemAsteroid : public AItemBase
{
	GENERATED_BODY()

public:
	// Called when hit by a projectile
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void PostActorCreated() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMesh*> AsteroidMeshes;
};
