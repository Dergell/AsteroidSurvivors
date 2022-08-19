// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "GameplayAbility_AttackManual.generated.h"

class UAbilityTask_Repeat;
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

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void OnPerformAction(int32 ActionNumber);
	UFUNCTION()
	void OnFinished(int32 ActionNumber);

	UPROPERTY(EditDefaultsOnly, Category = "Ability: Attack Manual")
	TSubclassOf<AItemProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Ability: Attack Manual")
	float TimeBetweenShots = 0.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Ability: Attack Manual")
	int32 ShotsPerActivation = 3;

	UPROPERTY(BlueprintReadOnly)
	UAbilityTask_Repeat* RepeatTask;
};
