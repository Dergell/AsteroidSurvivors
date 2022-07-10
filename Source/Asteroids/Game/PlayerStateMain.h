/* Asteroid Survivors - Casual Rogue-Lite Indie Game
 * Copyright (C) 2022 Tony Schmich
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

	// Stores the current player score
	int32 Score = 0;

};
