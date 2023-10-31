// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AsteroidsSaveGame.generated.h"

/**
 * This class is holding all data meant to be saved to disk
 */
UCLASS()
class ASTEROIDSURVIVORS_API UAsteroidsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	int32 GetHighScore() const;
	void SetHighScore(int32 InHighScore);
	int32 GetHighTime() const;
	void SetHighTime(int32 InHighTime);

private:
	// The best score achieved on this local computer
	UPROPERTY()
	int32 HighScore = 0;

	// The best time achieved on this local computer
	UPROPERTY()
	int32 HighTime = 0;
};