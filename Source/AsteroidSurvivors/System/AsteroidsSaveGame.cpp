// Asteroid Survivors - Copyright (C) 2023 Tony Schmich


#include "AsteroidsSaveGame.h"

int32 UAsteroidsSaveGame::GetHighScore() const
{
	return HighScore;
}

void UAsteroidsSaveGame::SetHighScore(int32 InHighScore)
{
	if (InHighScore > HighScore)
	{
		HighScore = InHighScore;
	}
}

int32 UAsteroidsSaveGame::GetHighTime() const
{
	return HighTime;
}

void UAsteroidsSaveGame::SetHighTime(int32 InHighTime)
{
	if (InHighTime > HighTime)
	{
		HighTime = InHighTime;
	}
}

void UAsteroidsSaveGame::GetVolumeSettings(int32& OutMusic, int32& OutEffects)
{
	OutMusic = VolumeMusic;
	OutEffects = VolumeEffects;
}

void UAsteroidsSaveGame::SetVolumeSettings(int32 InMusic, int32 InEffects)
{
	if (InMusic >= 0 && InMusic <= 100)
	{
		VolumeMusic = InMusic;
	}

	if (InEffects >= 0 && InEffects <= 100)
	{
		VolumeEffects = InEffects;
	}
}