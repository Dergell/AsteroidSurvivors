// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/ProjectileInterface.h"
#include "GameFramework/Pawn.h"
#include "GameplayEffectTypes.h"
#include "EnemyBase.generated.h"

class UFloatingPawnMovement;
class AItemWeightedSpawn;
class UFXSystemComponent;
class UGameplayEffect;
class UAttributeSetBase;
class UGameplayAbilityBase;
class UAbilitySystemComponent;
class UNiagaraComponent;
class AItemProjectile;
class UBehaviorTree;

UCLASS()
class ASTEROIDS_API AEnemyBase : public APawn, public IProjectileInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEnemyBase();
	virtual void Tick(float DeltaTime) override;

	// Interfaces
	virtual void HitByProjectile_Implementation(APawn* ProjectileInstigator, TSubclassOf<UGameplayEffect> ProjectileEffect) override;

	// Getter & Setter
	UFUNCTION(BlueprintGetter)
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	// Gameplay Ability System
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitializeAttributes();
	virtual void GiveAbilities();

protected:
	virtual void BeginPlay() override;

	// Gameplay Ability System
	void HealthChanged(const FOnAttributeChangeData& Data);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFloatingPawnMovement* MovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* ExplosionNiagaraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* ExplosionAudioComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UFXSystemComponent*> Engines;

	// AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Target;

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollLimit = 30.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItemProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItemWeightedSpawn> LootItemClass;

	// Gameplay Ability System
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbilityBase>> DefaultAbilities;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	UAttributeSetBase* Attributes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

private:
	// Actions
	void FaceTargetDirection(float DeltaTime);
	void Die();

	// Subscribers
	void OnExplosionFinished(UNiagaraComponent* PSystem);
};