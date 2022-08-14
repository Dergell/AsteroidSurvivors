// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AIMovementComponent.generated.h"

/**
 * This component will control the movement of AI pawns.
 */
UCLASS()
class ASTEROIDS_API UAIMovementComponent : public UFloatingPawnMovement
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
