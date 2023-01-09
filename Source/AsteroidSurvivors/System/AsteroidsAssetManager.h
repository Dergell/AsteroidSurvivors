// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AsteroidsAssetManager.generated.h"

/**
 * This singleton is used to initialize native gameplay tags.
 */
UCLASS()
class ASTEROIDSURVIVORS_API UAsteroidsAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UAsteroidsAssetManager();

	// Returns the AssetManager singleton object.
	static UAsteroidsAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};