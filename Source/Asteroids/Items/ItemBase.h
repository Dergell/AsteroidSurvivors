// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroids/Interfaces/ProjectileInterface.h"
#include "ItemBase.generated.h"

/**
 * Class which implements the main game base item. Extended by code and blueprints.
 */
UCLASS()
class ASTEROIDS_API AItemBase : public AActor, public IProjectileInterface
{
	GENERATED_BODY()

public:
	AItemBase();

	virtual void Tick(float DeltaSeconds) override;

	// Getters
	UFUNCTION(BlueprintPure)
	bool GetIsCollectable() const;
	UFUNCTION(BlueprintPure)
	int32 GetPointsValue() const;

	// Called when the item is collected by the player
	UFUNCTION(BlueprintNativeEvent)
	void Collected();

	// Called when hit by a projectile
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCollectable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointsValue = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 KillDistance = 10000;
};
