// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AvoidObstacles.generated.h"

struct FEnvQueryResult;
class UEnvQuery;

/**
 * This task will scan for obstacles and try to evade them
 */
UCLASS(Blueprintable)
class ASTEROIDSURVIVORS_API UBTTask_AvoidObstacles : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_AvoidObstacles(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// Triggers an update of Obstacles by FindObstaclesQuery EQS every UpdateObstaclesInterval seconds
	UFUNCTION()
	void UpdateObstacles(const UBehaviorTreeComponent* OwnerComp);

	// Updates Obstacles when EQS is done
	void HandleUpdateResult(TSharedPtr<FEnvQueryResult> Result);

	// How often should be scanned for obstacles in seconds
	UPROPERTY(EditAnywhere)
	float UpdateObstaclesInterval = 0.5f;

	// Higher values mean evasion happens faster and stronger
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0, UIMin = 0, ClampMax = 1, UIMax = 1))
	float EvasionIntensity = 1.f;

	// An EQS Query used to find all obstacles in a range
	UPROPERTY(EditAnywhere)
	UEnvQuery* FindObstaclesQuery;

	// The radius around the pawn beyond which obstacles are considered cleared
	UPROPERTY(EditAnywhere)
	float AcceptableRadius = 100.f;

	// List of obstacles found by EQS
	TArray<TWeakObjectPtr<AActor>> Obstacles;
};