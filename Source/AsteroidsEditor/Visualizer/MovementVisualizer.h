// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

/**
 * 
 */
class ASTEROIDSEDITOR_API FMovementVisualizer : public FComponentVisualizer
{
public:
	bool ShowWhenSelected() override;
	void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

};
