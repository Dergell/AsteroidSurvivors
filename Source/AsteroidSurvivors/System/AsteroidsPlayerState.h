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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Get AbilitySystemComponent
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Initialize attribute values defined by DefaultAttributeEffect
	virtual void InitializeAttributes();

	// Grant abilities defined by DefaultAbilities
	virtual void GiveAbilities();

	// Updates the players score and trigger UI update
	virtual void UpdateScore_Implementation(int32 Points) override;

	// Called once health depletes
	UFUNCTION(BlueprintNativeEvent)
	void Die();

	// Callback when the Health attribute changed
	void HealthChanged(const FOnAttributeChangeData& Data);

protected:
	// This will apply a default value to all attributes
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	// Default abilities
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAsteroidsGameplayAbility>> DefaultAbilities;

	// Used to implement gameplay abilities
	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;

private:
	// The attribute set of this ship
	UPROPERTY()
	UAsteroidsAttributeSet* Attributes;

	// The current score of the player
	UPROPERTY(VisibleInstanceOnly)
	int32 Score = 0;

	// Adds the tag Cheats.UnlimitedHealth
	UPROPERTY(EditDefaultsOnly)
	bool bIsInvincible = false;
};