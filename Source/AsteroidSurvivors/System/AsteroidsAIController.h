// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AsteroidsAIController.generated.h"

struct FAIStimulus;

/**
 * Class which implements the base AI controller. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AAsteroidsAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAsteroidsAIController(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	// Delegate callback for PerceptionComponent->OnTargetPerceptionUpdated
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// Timer callback for TargetRememberTimerHandle
	UFUNCTION()
	void OnTargetForgotten();

protected:
	// Seconds the AI will remember target position after losing sight
	UPROPERTY(EditAnywhere)
	float TargetRememberTime = 5.f;

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree;

	FTimerHandle TargetRememberTimerHandle;
};