// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "GameplayAbility_AttackManual.generated.h"

class AItemProjectile;

/**
 * This ability allows manual shooting in forward facing direction.
 */
UCLASS()
class ASTEROIDS_API UGameplayAbility_AttackManual : public UGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGameplayAbility_AttackManual();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<AItemProjectile> ProjectileClass;
};
