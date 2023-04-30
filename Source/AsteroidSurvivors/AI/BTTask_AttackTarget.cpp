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
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIShip* Owner = OwnerComp.GetAIOwner()->GetPawn<AAIShip>();
	if (Owner == nullptr)
	{
		return EBTNodeResult::Failed;
	}

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
			return EBTNodeResult::Failed;
		}

		UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
		if (TargetASC->HasMatchingGameplayTag(FAsteroidsGameplayTags::Get().State_Dead))
		{
			return EBTNodeResult::Failed;
		}
	}

	const FGameplayTagContainer Tags = FGameplayTagContainer(FAsteroidsGameplayTags::Get().Ability_Attack);
	TArray<FGameplayAbilitySpec*> Abilities;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tags, Abilities);
	for (const FGameplayAbilitySpec* Ability : Abilities)
	{
		if (!AbilitySystemComponent->TryActivateAbility(Ability->Handle))
		{
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Succeeded;
}