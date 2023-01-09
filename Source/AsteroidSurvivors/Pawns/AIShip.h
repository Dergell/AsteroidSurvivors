// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Pawns/SpaceShip.h"
#include "AIShip.generated.h"

class AProjectile;
class AWeightedSpawn;
class UAsteroidsAttributeSet;
class UFXSystemComponent;
class UAsteroidsGameplayAbility;
class UGameplayEffect;
struct FOnAttributeChangeData;

UCLASS()
class ASTEROIDSURVIVORS_API AAIShip : public ASpaceShip
{
	GENERATED_BODY()

public:
	AAIShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get AbilitySystemComponent from player controller
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Applies the DefaultAttributeEffect
	virtual void InitializeAttributes();

	// Grants the DefaultAbilities
	virtual void GiveAbilities();

	// Callback when the Health Attribute changed
	void HealthChanged(const FOnAttributeChangeData& Data);

private:
	// Calculate deflection angle and rotate towards it
	void FaceTargetDirection(float DeltaTime);

	// Called once health depletes
	void Die();

protected:
	// Class of projectiles this ship will shoot
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileClass;

	// Class that determines loot dropped by this ship
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeightedSpawn> LootItemClass;

	// This will apply a default value to all attributes
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	// Default abilities
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAsteroidsGameplayAbility>> DefaultAbilities;

	// List of engine effects added through the editor and tagged with "Engine"
	UPROPERTY(BlueprintReadOnly)
	TArray<UFXSystemComponent*> Engines;

	// Used to implement gameplay abilities
	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;

private:
	// The attribute set of this ship
	UPROPERTY()
	UAsteroidsAttributeSet* Attributes;
};