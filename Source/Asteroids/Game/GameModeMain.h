// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeMain.generated.h"

/**
 * Class which implements the main game mode. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API AGameModeMain : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
