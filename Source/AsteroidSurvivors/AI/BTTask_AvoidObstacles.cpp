// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "BTTask_AvoidObstacles.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Pawns/AIShip.h"

UBTTask_AvoidObstacles::UBTTask_AvoidObstacles(const FObjectInitializer& ObjectInitializer)
{
	bNotifyTick = true;
	bIgnoreRestartSelf = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_AvoidObstacles::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("UpdateObstacles"), &OwnerComp);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, UpdateObstaclesInterval, true);

	return EBTNodeResult::InProgress;
}

void UBTTask_AvoidObstacles::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!OwnerComp.GetAIOwner())
	{
		return;
	}

	const AAIShip* Pawn = OwnerComp.GetAIOwner()->GetPawn<AAIShip>();
	if (!IsValid(Pawn))
	{
		return;
	}

	// When there's nothing to evade, we are done
	if (Obstacles.IsEmpty())
	{
		return;
	}

	FVector EvasionDirection = FVector::ZeroVector;
	for (int Index = Obstacles.Num() - 1; Index >= 0; --Index)
	{
		if (!Obstacles.IsValidIndex(Index))
		{
			continue;
		}

		AActor* Obstacle = Obstacles[Index].Get();
		if (!IsValid(Obstacle))
		{
			Obstacles.Remove(Obstacle);
			continue;
		}

		// Remove the obstacle when evaded
		if (Pawn->GetDistanceTo(Obstacle) >= AcceptableRadius)
		{
			Obstacles.Remove(Obstacle);
			continue;
		}

		// Add the vector away form this obstacle to the evasion direction
		EvasionDirection += Pawn->GetActorLocation() - Obstacle->GetActorLocation();
	}

	// Try to evade the obstacles
	EvasionDirection = EvasionDirection.GetSafeNormal() * EvasionIntensity;
	UPawnMovementComponent* Movement = Pawn->GetMovementComponent();
	if (Movement != nullptr)
	{
		Movement->AddInputVector(EvasionDirection, true);
	}
}

void UBTTask_AvoidObstacles::UpdateObstacles(const UBehaviorTreeComponent* OwnerComp)
{
	if (OwnerComp == nullptr || OwnerComp->GetAIOwner() == nullptr)
	{
		return;
	}

	FEnvQueryRequest QueryRequest = FEnvQueryRequest(FindObstaclesQuery, OwnerComp->GetAIOwner());
	QueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UBTTask_AvoidObstacles::HandleUpdateResult);
}

void UBTTask_AvoidObstacles::HandleUpdateResult(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsSuccessful())
	{
		TArray<AActor*> Actors;
		Result->GetAllAsActors(Actors);
		for (AActor* Actor : Actors)
		{
			Obstacles.AddUnique(Actor);
		}
	}
}