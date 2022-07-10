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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Asteroid Survivors")
	void UpdateScore(int32 Points);

};
