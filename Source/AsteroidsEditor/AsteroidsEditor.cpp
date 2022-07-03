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


#include "AsteroidsEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "Visualizer/MovementVisualizer.h"

IMPLEMENT_GAME_MODULE(FAsteroidsEditorModule, AsteroidsEditor);

void FAsteroidsEditorModule::StartupModule()
{
	if (GUnrealEd) {
		TSharedPtr<FMovementVisualizer> MovementVisualizer = MakeShareable(new FMovementVisualizer);
		if (MovementVisualizer.IsValid()) {
			GUnrealEd->RegisterComponentVisualizer(UStaticMeshComponent::StaticClass()->GetFName(), MovementVisualizer);
		}
	}
}

void FAsteroidsEditorModule::ShutdownModule()
{
	if (GUnrealEd) {
		GUnrealEd->UnregisterComponentVisualizer(UStaticMeshComponent::StaticClass()->GetFName());
	}
}