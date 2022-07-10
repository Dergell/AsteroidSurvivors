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


#include "ItemBase.h"

AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

bool AItemBase::GetIsCollectable()
{
	return IsCollectable;
}

int32 AItemBase::GetPointsValue()
{
	return PointsValue;
}

void AItemBase::Collected_Implementation()
{
	Destroy();
}

void AItemBase::HitByProjectile_Implementation()
{
	Destroy();
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}