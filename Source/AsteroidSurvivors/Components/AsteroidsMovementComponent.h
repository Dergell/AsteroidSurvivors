// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AsteroidsMovementComponent.generated.h"

/*
 * Handles pawn movement in zero gravity restricted to the XY plane.
 */
UCLASS(ClassGroup=Movement, meta=(BlueprintSpawnableComponent))
class ASTEROIDSURVIVORS_API UAsteroidsMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAsteroidsMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Return value of MaxSpeed member
	virtual float GetMaxSpeed() const override;

	// Sets the rotation target calculated towards a location
	UFUNCTION(BlueprintCallable)
	void RotateTowardsLocation(const FVector Location);

protected:
	// Apply control input as force to pawn
	void ApplyControlInput();

private:
	// Speed of rotation towards the target rotation 
	UPROPERTY(EditAnywhere, meta=(ClampMin = "1", UIMin = "1"))
	float MaxSpeed = 1000.f;

	// Speed of rotation towards the target rotation 
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 1.f;

	// Limit how many degrees to roll during rotation
	UPROPERTY(EditAnywhere)
	float RollLimit = 30.f;

	// Modifies how fast velocity is changed 
	UPROPERTY(EditAnywhere)
	float Acceleration = 1000.f;
};