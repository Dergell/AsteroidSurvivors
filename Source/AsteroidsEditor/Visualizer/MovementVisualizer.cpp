// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "MovementVisualizer.h"

bool FMovementVisualizer::ShowWhenSelected() { return false; }

void FMovementVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Component);
	if (Mesh) {
		PDI->DrawLine(
			Mesh->GetComponentLocation(),
			Mesh->GetComponentLocation() + Mesh->GetComponentVelocity(),
			FLinearColor::Blue,
			SDPG_Foreground
		);
	}
}
