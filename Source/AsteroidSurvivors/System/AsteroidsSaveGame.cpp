// Asteroid Survivors - Copyright (C) 2023 Tony Schmich


#include "AsteroidsSaveGame.h"

int32 UAsteroidsSaveGame::GetHighScore() const
{
	return HighScore;
}

void UAsteroidsSaveGame::SetHighScore(int32 InHighScore)
{
	HighScore = InHighScore;
}

int32 UAsteroidsSaveGame::GetHighTime() const
{
	return HighTime;
}

void UAsteroidsSaveGame::SetHighTime(int32 InHighTime)
{
	HighTime = InHighTime;
}