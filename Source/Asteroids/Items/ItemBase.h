// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Asteroids/Interfaces/ProjectileHitTriggerInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

/**
 * Class which implements the main game base item. Extended by code and blueprints.
 */
UCLASS()
class ASTEROIDS_API AItemBase : public AActor, public IProjectileHitTriggerInterface
{
	GENERATED_BODY()
	
public:	
	
	AItemBase();

	/** The static mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	/** Called when hit by a projectile */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger Reaction")
	void HitByProjectile(); virtual void HitByProjectile_Implementation() override;

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called when this item is collected by the player */
	UFUNCTION(BlueprintNativeEvent)
	void Collected();
};
