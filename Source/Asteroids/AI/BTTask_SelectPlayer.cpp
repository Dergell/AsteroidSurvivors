// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "BTTask_SelectPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_SelectPlayer::UBTTask_SelectPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerPawnKey = TEXT("PlayerPawn");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_SelectPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp && !PlayerPawnKey.IsNone())
	{
		BlackboardComp->SetValueAsObject(PlayerPawnKey, UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
