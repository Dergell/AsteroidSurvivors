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
	virtual void Tick(float DeltaSeconds) override;

	// Getters & Setters
	UFUNCTION(BlueprintPure)
	bool GetIsCollectable() const;
	UFUNCTION(BlueprintPure)
	int32 GetPointsValue() const;

	// Actions
	UFUNCTION(BlueprintNativeEvent)
	void Collected();

	// Interfaces
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator, FGameplayEffectSpecHandle EffectSpec) override;

protected:
	virtual void BeginPlay() override;

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCollectable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointsValue = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 KillDistance = 10000;
};