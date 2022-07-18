// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerStateMain.h"

void APlayerStateMain::UpdateScore_Implementation(int32 Points)
{
	// Update the score
	Score += Points;

	// Tell the player controller to update everything that uses the current score
	IItemInterface* Interface = Cast<IItemInterface>(GetPlayerController());
	if (Interface)
		Interface->Execute_UpdateScore(GetPlayerController(), Score);
}
