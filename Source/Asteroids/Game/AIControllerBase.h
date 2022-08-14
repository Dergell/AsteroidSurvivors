// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

class UBehaviorTreeComponent;

/**
 * Class which implements the base AI controller. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
