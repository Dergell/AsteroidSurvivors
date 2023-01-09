// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackTarget.generated.h"

/**
 * This Task will activate all offensive abilities.
 */
UCLASS()
class ASTEROIDSURVIVORS_API UBTTask_AttackTarget : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;
};