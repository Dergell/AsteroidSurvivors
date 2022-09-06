// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "AIControllerBase.h"

#include "Asteroids/AI/EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const AEnemyBase* Enemy = Cast<AEnemyBase>(InPawn);
	if (Enemy) {
		RunBehaviorTree(Enemy->GetBehaviorTree());
		
		AActor* Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Target) {
			GetBlackboardComponent()->SetValueAsObject("Target", Target);
			GetBlackboardComponent()->SetValueAsFloat("DistanceToTarget", Target->GetDistanceTo(InPawn));
		}
	}
}
