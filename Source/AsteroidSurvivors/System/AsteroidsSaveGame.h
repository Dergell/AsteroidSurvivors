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

	void GetVolumeSettings(int32 &OutMusic, int32 &OutEffects);
	void SetVolumeSettings(int32 InMusic, int32 InEffects);

private:
	// Statistics
	UPROPERTY()
	int32 HighScore = 0;
	UPROPERTY()
	int32 HighTime = 0;

	// Volume settings
	UPROPERTY()
	int32 VolumeMusic = 80;
	UPROPERTY()
	int32 VolumeEffects = 80;
};