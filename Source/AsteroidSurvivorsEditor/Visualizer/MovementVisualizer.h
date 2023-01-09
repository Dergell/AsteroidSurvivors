// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

/**
 * 
 */
class ASTEROIDSURVIVORSEDITOR_API FMovementVisualizer : public FComponentVisualizer
{
public:
	bool ShowWhenSelected() override;
	void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

};