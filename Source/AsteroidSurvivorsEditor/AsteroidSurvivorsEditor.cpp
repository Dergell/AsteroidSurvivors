// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidSurvivorsEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "Visualizer/MovementVisualizer.h"

IMPLEMENT_GAME_MODULE(FAsteroidSurvivorsEditorModule, AsteroidSurvivorsEditor);

void FAsteroidSurvivorsEditorModule::StartupModule()
{
	if (GUnrealEd) {
		TSharedPtr<FMovementVisualizer> MovementVisualizer = MakeShareable(new FMovementVisualizer);
		if (MovementVisualizer.IsValid()) {
			GUnrealEd->RegisterComponentVisualizer(UStaticMeshComponent::StaticClass()->GetFName(), MovementVisualizer);
		}
	}
}

void FAsteroidSurvivorsEditorModule::ShutdownModule()
{
	if (GUnrealEd) {
		GUnrealEd->UnregisterComponentVisualizer(UStaticMeshComponent::StaticClass()->GetFName());
	}
}