// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Framework for item interactions. Extended by blueprints.
 */
class ASTEROIDS_API IItemInterface
{
	GENERATED_BODY()

public:
	// Called when a collectible item is collected
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateScore(int32 Points);
};
