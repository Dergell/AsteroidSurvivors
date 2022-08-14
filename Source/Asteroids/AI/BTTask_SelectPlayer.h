// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SelectPlayer.generated.h"

/**
 * This Task will set the Pawn controlled by the Player to a BB Key
 */
UCLASS()
class ASTEROIDS_API UBTTask_SelectPlayer : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PlayerPawnKey;
};
