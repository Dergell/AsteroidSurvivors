// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidsAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAsteroidsAIController::AAsteroidsAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AAsteroidsAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update DistanceToTarget
	const AActor* Target = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (IsValid(Target))
	{
		const float Distance = Target->GetDistanceTo(GetPawn());
		GetBlackboardComponent()->SetValueAsFloat(TEXT("DistanceToTarget"), Distance);
	}
}

ETeamAttitude::Type AAsteroidsAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const AController* Controller = Other.GetInstigatorController();
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(Controller);
	if (OtherTeamAgent && GetGenericTeamId() != OtherTeamAgent->GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return Super::GetTeamAttitudeTowards(Other);
}

void AAsteroidsAIController::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAsteroidsAIController::OnPerceptionUpdated);
}

void AAsteroidsAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void AAsteroidsAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		// Set sensed actor as target
		SetFocus(Actor);
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);

		// Clear the TargetRememberTimerHandle so we don't forget a recovered target
		if (GetWorldTimerManager().TimerExists(TargetRememberTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(TargetRememberTimerHandle);
		}
	}
	else
	{
		// Unset target
		ClearFocus(EAIFocusPriority::Gameplay);
		GetBlackboardComponent()->ClearValue(TEXT("Target"));

		// Remember last position for searching
		GetBlackboardComponent()->SetValueAsVector(TEXT("TargetPosition"), Stimulus.StimulusLocation);

		// Start timer to forget the target
		GetWorldTimerManager().SetTimer(TargetRememberTimerHandle, this, &AAsteroidsAIController::OnTargetForgotten, TargetRememberTime);
	}
}

void AAsteroidsAIController::OnTargetForgotten()
{
	GetBlackboardComponent()->ClearValue(TEXT("TargetPosition"));
}