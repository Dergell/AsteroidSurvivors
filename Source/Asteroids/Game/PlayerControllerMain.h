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
#include "GameFramework/PlayerController.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "WidgetMain.h"
#include "PlayerControllerMain.generated.h"

/**
 * Class which implements the main player controller. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API APlayerControllerMain : public APlayerController, public IItemInterface
{
	GENERATED_BODY()

public:

	// Main UI widget class
	UPROPERTY(EditDefaultsOnly, Category = "Asteroid Survivors")
	TSubclassOf<class UWidgetMain> MainWidgetClass;

	// Interface called when score needs to be updated
	void UpdateScore_Implementation(int32 Points) override;

private:

	// Main UI widget for the player
	UWidgetMain* MainWidget;

	// Called when the controller possesses a pawn
	void OnPossess(APawn* aPawn) override;

};
