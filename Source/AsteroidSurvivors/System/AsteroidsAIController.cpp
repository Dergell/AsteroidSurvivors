// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidsAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AAsteroidsAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);

		AActor* Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Target)
		{
			SetFocus(Target);
			GetBlackboardComponent()->SetValueAsObject("Target", Target);
			GetBlackboardComponent()->SetValueAsFloat("DistanceToTarget", Target->GetDistanceTo(InPawn));
		}
	}
}