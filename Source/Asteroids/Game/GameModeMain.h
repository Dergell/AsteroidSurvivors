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

	/** Stores the current points */
	int32 Points = 0;

	/** Property which stores the class of widget to use */
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<class UWidgetMain> GameWidgetClass;

	/** The GameWidget itself */
	UPROPERTY()
	class UWidgetMain* GameWidget;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Update the item text */
	void UpdateItemText();

public:

	/** Will be called when an item has been collected */
	void ItemCollected();
};
