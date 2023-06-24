// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "GameplayAbility_AttackManual.h"

#include "Abilities/Tasks/AbilityTask_Repeat.h"
#include "AbilitySystemComponent.h"
#include "AsteroidsAttributeSet.h"
#include "Actors/Projectile.h"
#include "Components/ArrowComponent.h"
#include "Pawns/SpaceShip.h"

void UGameplayAbility_AttackManual::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGameplayAbility_AttackManual::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !IsValid(ProjectileClass))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}

		RepeatTask = UAbilityTask_Repeat::RepeatAction(this, TimeBetweenShots, ShotsPerActivation);
		RepeatTask->OnPerformAction.AddDynamic(this, &UGameplayAbility_AttackManual::OnPerformAction);
		RepeatTask->OnFinished.AddDynamic(this, &UGameplayAbility_AttackManual::OnFinished);
		RepeatTask->ReadyForActivation();
	}
}

void UGameplayAbility_AttackManual::OnFinished(int32 ActionNumber)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGameplayAbility_AttackManual::OnPerformAction(int32 ActionNumber)
{
	APawn* Pawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());
	if (Pawn == nullptr)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Pawn;
	SpawnParams.Instigator = Pawn->GetInstigator();

	TArray<UActorComponent*> Muzzles = Pawn->GetComponentsByTag(UArrowComponent::StaticClass(), FName("Muzzle"));
	for (UActorComponent* Item : Muzzles)
	{
		UArrowComponent* Muzzle = Cast<UArrowComponent>(Item);
		FVector MuzzleLocation = Muzzle->GetComponentLocation();
		FRotator MuzzleRotation = Muzzle->GetComponentRotation();

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

		const ASpaceShip* OwnerShip = Cast<ASpaceShip>(Pawn);
		if (IsValid(Projectile) && IsValid(OwnerShip))
		{
			const UAbilitySystemComponent* OwnerASC = OwnerShip->GetAbilitySystemComponent();
			const UAsteroidsAttributeSet* Attributes = Cast<UAsteroidsAttributeSet>(OwnerASC->GetAttributeSet(UAsteroidsAttributeSet::StaticClass()));
			Projectile->MultiplyEffectAmount(Attributes->GetDamageMultiplier());
		}
	}
}