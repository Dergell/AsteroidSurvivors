// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class USphereComponent;

/**
 * Class which implements the a spawn volume. Extended by blueprints.
 * This actor allows spawning of actors up to a specific number inside the volume while moving it around.
 */
UCLASS()
class ASTEROIDSURVIVORS_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Attach this SpawnVolume to an actor
	void AttachToActor(AActor* AttachToActor);
	
private:
	// Get the number of missing SpawnActorClass in the volume
	int8 GetMissingActorsCount() const;

	// Used to spawn <Count> actors inside the volume at random locations
	void SpawnActors(int8 Count) const;

	// If the volume is attached to an actor, is will circle around it
	void ContinueMovement(const float DeltaTime);

protected:
	// The volume to check an spawn missing actors
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* Volume;

	// Actors will be spawned up to this number
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxAllowedActors = 0;

	// The actor class to check and spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> SpawnActorClass;

	// The volume can be attached to an actor for relative positioning
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	AActor* AttachedToActor;

	// Should the volume circle around the attached actor or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldMoveAroundAttachedActor = true;

	// How many degrees should the volume circle per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DegreePerSecond = 360;

private:
	// Initial distance to the attached actor
	float DistanceToAttachedActor = 0;
};