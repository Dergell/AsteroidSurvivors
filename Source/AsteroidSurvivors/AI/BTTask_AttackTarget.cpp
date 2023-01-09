// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "BTTask_AttackTarget.h"

#include "AIController.h"
#include "AbilitySystem/AsteroidsGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pawns/AIShip.h"
#include "Pawns/PlayerShip.h"

UBTTask_AttackTarget::UBTTask_AttackTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_AttackTarget, TargetKey), APawn::StaticClass());
	TargetKey.SelectedKeyName = TEXT("Target");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	if (const AAIShip* Owner = OwnerComp.GetAIOwner()->GetPawn<AAIShip>())
	{
		UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent();
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

		APlayerShip* Target = Cast<APlayerShip>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
		if (Target)
		{
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Target);
			Params.AddIgnoredActor(Owner);

			bool TraceResult = GetWorld()->LineTraceSingleByChannel(Hit, Owner->GetActorLocation(),
				Target->GetActorLocation(), ECC_PhysicsBody, Params);

			if (TraceResult)
			{
				return Result;
			}
		}

		const FGameplayTagContainer Tags = FGameplayTagContainer(FAsteroidsGameplayTags::Get().Ability_Attack);
		TArray<FGameplayAbilitySpec*> Abilities;
		AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tags, Abilities);

		for (const FGameplayAbilitySpec* Ability : Abilities)
		{
			if (AbilitySystemComponent->TryActivateAbility(Ability->Handle))
			{
				Result = EBTNodeResult::Succeeded;
			}
		}
	}

	return Result;
}