// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AsteroidsAIController.generated.h"

/**
 * Class which implements the base AI controller. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AAsteroidsAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree;
};