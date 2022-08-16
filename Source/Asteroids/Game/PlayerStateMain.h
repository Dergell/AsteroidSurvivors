// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "PlayerStateMain.generated.h"

class UGameplayAbilityBase;
class UGameplayEffect;
class UAttributeSetBase;

/**
 * Class which implements the main player state. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API APlayerStateMain : public APlayerState, public IItemInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APlayerStateMain();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Called when the score needs to be updated
	void UpdateScore_Implementation(int32 Points) override;

	// Gameplay Ability System
	virtual void InitializeAttributes();
	virtual void GiveAbilities();

protected:
	// Gameplay Ability System
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbilityBase>> DefaultAbilities;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	UAttributeSetBase* Attributes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

private:
	int32 Score = 0;
};
