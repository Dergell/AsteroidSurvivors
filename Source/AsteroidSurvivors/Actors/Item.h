// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Actors/ProjectileInterface.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

/**
 * Class which implements the main game base item. Extended by code and blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AItem : public AActor, public IProjectileInterface
{
	GENERATED_BODY()

public:
	AItem();

	// Getters & Setters
	UFUNCTION(BlueprintPure)
	bool GetIsCollectable() const;

	// Actions
	UFUNCTION(BlueprintCallable)
	void MultiplyEffectAmount(float Multiplier);
	UFUNCTION(BlueprintCallable)
	int32 Collect(TSubclassOf<UGameplayEffect>& OutGameplayEffect, float& OutEffectAmount);

	// Events
	UFUNCTION(BlueprintNativeEvent)
	void Collected();

	// Interfaces
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator, FGameplayEffectSpecHandle EffectSpec) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void KillCheck();

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Asteroids)
	bool IsCollectable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Asteroids)
	int32 PointsValue = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Asteroids)
	int64 KillDistance = 10000;

	// GameplayEffect applied by this item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Asteroids)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Asteroids)
	float EffectAmount;

private:
	UPROPERTY()
	FTimerHandle KillCheckTimer;
};