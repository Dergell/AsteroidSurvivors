﻿// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidsMovementComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAsteroidsMovementComponent::UAsteroidsMovementComponent()
{
	bConstrainToPlane = true;
	Super::SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
}

void UAsteroidsMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedPrimitive)
	{
		return;
	}

	Velocity = UpdatedPrimitive->GetComponentVelocity();

	const AController* Controller = PawnOwner->GetController();
	if (Controller && Controller->IsLocalController())
	{
		// apply input for local players but also for AI that's not following a navigation path at the moment
		if (Controller->IsLocalPlayerController() == true || Controller->IsFollowingAPath() == false || bUseAccelerationForPaths)
		{
			ApplyControlInput();
		}
		// if it's not player controller, but we do have a controller, then it's AI
		// (that's not following a path) and we need to limit the speed
		else if (IsExceedingMaxSpeed(MaxSpeed) == true)
		{
			Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
		}

		// Finalize
		UpdateComponentVelocity();
	}
}

float UAsteroidsMovementComponent::GetMaxSpeed() const
{
	return MaxSpeed;
}

void UAsteroidsMovementComponent::RotateTowardsLocation(const FVector Location, const float DeltaTime)
{
	const FRotator CurrentRotation = UpdatedPrimitive->GetRelativeRotation();
	const FVector TargetVector = Location - GetOwner()->GetActorLocation();
	FRotator RotationTarget = TargetVector.Rotation();

	// Add some roll to the turn
	RotationTarget.Roll = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, RotationTarget.Yaw);
	// Clamp roll so we don't overshoot
	RotationTarget.Roll = FMath::Clamp(RotationTarget.Roll, -RollLimit, RollLimit);
	// Cancel any pitch by physics
	RotationTarget.Pitch = 0;

	// For the actual turn, just add the direction to the existing yaw
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, RotationTarget, DeltaTime, RotationSpeed);

	// Normalize and set to primitive
	UpdatedPrimitive->SetRelativeRotation(NewRotation);
}

void UAsteroidsMovementComponent::ApplyControlInput()
{
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);
	const float AnalogInputModifier = (ControlAcceleration.SizeSquared() > 0.f ? ControlAcceleration.Size() : 0.f);

	// Apply force
	if (AnalogInputModifier > 0.f)
	{
		UpdatedPrimitive->AddForce(ControlAcceleration * Acceleration, NAME_None, true);
	}

	// Clamp to max speed
	if (IsExceedingMaxSpeed(GetMaxSpeed()))
	{
		FVector PhysicsVelocity = UpdatedPrimitive->GetPhysicsLinearVelocity();
		PhysicsVelocity = PhysicsVelocity.GetClampedToMaxSize(GetMaxSpeed());
		UpdatedPrimitive->SetPhysicsLinearVelocity(PhysicsVelocity);
	}

	ConsumeInputVector();
}