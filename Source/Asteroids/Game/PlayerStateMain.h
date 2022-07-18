// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "PlayerStateMain.generated.h"

/**
 * Class which implements the main player state. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API APlayerStateMain : public APlayerState, public IItemInterface
{
	GENERATED_BODY()

public:
	// Called when the score needs to be updated
	void UpdateScore_Implementation(int32 Points) override;

private:
	int32 Score = 0;
};
