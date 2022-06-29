// Fill out your copyright notice in the Description page of Project Settings.

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