// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "ProjectileInterface.h"

FVector IProjectileInterface::CalculateLeadLocation(FVector OriginLocation, FVector TargetLocation,
	FVector TargetVelocity, float ProjectileSpeed)
{
	// Current distance to target
	float Distance = FVector::Dist(TargetLocation, OriginLocation);

	// First estimate, using current distance
	FVector Position = TargetLocation + TargetVelocity * 1.15 * (Distance / ProjectileSpeed);
	Distance = FVector::Dist(Position, OriginLocation);

	// Second estimate, using distance from first estimate
	Position = TargetLocation + TargetVelocity * 1.15 * (Distance / ProjectileSpeed);
	Distance = FVector::Dist(Position, OriginLocation);

	// Third estimate, using distance from second estimate
	return TargetLocation + TargetVelocity * 1.15 * (Distance / ProjectileSpeed);
}
