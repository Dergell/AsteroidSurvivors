// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Actors/ItemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GameplayEffectTypes.h"
#include "AsteroidsPlayerState.generated.h"

class UAsteroidsAttributeSet;
class UAsteroidsGameplayAbility;
class UGameplayEffect;

/**
 * Class which implements the main player state. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AAsteroidsPlayerState : public APlayerState, public IItemInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAsteroidsPlayerState();

	// Interfaces
	virtual void UpdateScore_Implementation(int32 Points) override;

	// Gameplay Ability System
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitializeAttributes();
	virtual void GiveAbilities();

protected:
	virtual void BeginPlay() override;

	// Actions
	UFUNCTION(BlueprintNativeEvent)
	void Die();

	// Gameplay Ability System
	void HealthChanged(const FOnAttributeChangeData& Data);

protected:
	// Gameplay Ability System
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UAsteroidsGameplayAbility>> DefaultAbilities;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	UAsteroidsAttributeSet* Attributes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

private:
	int32 Score = 0;
};