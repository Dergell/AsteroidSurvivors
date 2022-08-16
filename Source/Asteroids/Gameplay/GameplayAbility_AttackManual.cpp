// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "GameplayAbility_AttackManual.h"

#include "Asteroids/Items/ItemProjectile.h"
#include "Components/ArrowComponent.h"

UGameplayAbility_AttackManual::UGameplayAbility_AttackManual()
{
	AbilityInputID = EAbilityInputID::AttackManual;
}

void UGameplayAbility_AttackManual::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			return;
		}

		APawn* Pawn = CastChecked<APawn>(ActorInfo->AvatarActor.Get());
		if (Pawn == nullptr)
		{
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

			GetWorld()->SpawnActor<AItemProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
