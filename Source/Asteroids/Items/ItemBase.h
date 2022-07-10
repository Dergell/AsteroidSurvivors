/* Asteroid Survivors - Casual Rogue-Lite Indie Game
 * Copyright (C) 2022 Tony Schmich
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Asteroid Survivors")
	bool GetIsCollectable();
	UFUNCTION(BlueprintCallable, Category = "Asteroid Survivors")
	int32 GetPointsValue();

	// Called when the item is collected by the player
	UFUNCTION(BlueprintNativeEvent, Category = "Asteroid Survivors")
	void Collected();

	// Called when hit by a projectile
	void HitByProjectile_Implementation() override;

protected:
	
	// The static mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asteroid Survivors")
	UStaticMeshComponent* Mesh;

	// Whether this item collectable by the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Survivors")
	bool IsCollectable;

	// Points value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid Survivors")
	int32 PointsValue;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
