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


#include "GameModeMain.h"
#include "WidgetMain.h"
#include "Asteroids/Items/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void AGameModeMain::BeginPlay()
{
	// Collect all Items of class ItemBase
	TArray<AActor*> Items;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemBase::StaticClass(), Items);

	// Make sure we have a GameWidgetClass
	if (GameWidgetClass) {
		GameWidget = Cast<UWidgetMain>(CreateWidget(GetWorld(), GameWidgetClass));

		// Make sure we have a GameWidget
		if (GameWidget) {
			// Add it to the Viewport and update it
			GameWidget->AddToViewport();
			UpdateItemText();
		}
	}
}

void AGameModeMain::UpdateItemText()
{
	// Update current points in GameWidget
	GameWidget->UpdatePoints(Points);
}

void AGameModeMain::ItemCollected()
{
	// Increase Points and update the GameWidget
	Points += 500;
	UpdateItemText();
}
