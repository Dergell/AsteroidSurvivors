// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Asteroids/Interfaces/ProjectileInterface.h"
#include "GameFramework/Pawn.h"
#include "GameplayEffectTypes.h"
#include "EnemyBase.generated.h"

class UGameplayEffect;
class UAttributeSetBase;
class UGameplayAbilityBase;
class UAbilitySystemComponent;
class UNiagaraComponent;
class AItemProjectile;
class UAIMovementComponent;
class UBehaviorTree;

UCLASS()
class ASTEROIDS_API AEnemyBase : public APawn, public IProjectileInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when hit by a projectile
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator,
		TSubclassOf<UGameplayEffect> ProjectileEffect) override;

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	// Gameplay Ability System
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitializeAttributes();
	virtual void GiveAbilities();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIMovementComponent* MovementComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* ExplosionNiagaraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* ExplosionAudioComponent;

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

	// Gameplay Ability System
	void HealthChanged(const FOnAttributeChangeData& Data);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbilityBase>> DefaultAbilities;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	UAttributeSetBase* Attributes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

private:
	void FaceTargetDirection(float DeltaTime);
	void Die();

	UFUNCTION()
	void OnExplosionFinished(UNiagaraComponent* PSystem);
};
