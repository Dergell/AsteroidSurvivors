// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Asteroids/Interfaces/ProjectileInterface.h"
#include "GameFramework/Pawn.h"
#include "EnemyBase.generated.h"

class UNiagaraComponent;
class AItemProjectile;
class UAIMovementComponent;
class UBehaviorTree;

UCLASS()
class ASTEROIDS_API AEnemyBase : public APawn, public IProjectileInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when hit by a projectile
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator) override;

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIMovementComponent* MovementComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* ExplosionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollLimit = 30.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItemProjectile> ProjectileClass;

private:
	void FaceTargetDirection(float DeltaTime);
	
	UFUNCTION()
	void OnExplosionFinished(UNiagaraComponent* PSystem);
};
