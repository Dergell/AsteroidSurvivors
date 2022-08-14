// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "AIControllerBase.h"

#include "Asteroids/AI/EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const AEnemyBase* Enemy = Cast<AEnemyBase>(InPawn);
	if (Enemy)
	{
		RunBehaviorTree(Enemy->GetBehaviorTree());
	}
}
