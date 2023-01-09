// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AIShip.h"

#include "AbilitySystem/AsteroidsGameplayTags.h"
#include "AbilitySystem/AsteroidsAttributeSet.h"
#include "AbilitySystem/AsteroidsGameplayAbility.h"
#include "AbilitySystem/GameplayAbility_AttackManual.h"
#include "AbilitySystemComponent.h"
#include "Actors/Projectile.h"
#include "Actors/WeightedSpawn.h"
#include "Components/AsteroidsMovementComponent.h"
#include "NiagaraComponent.h"
#include "System/AsteroidsAIController.h"

// Sets default values
AAIShip::AAIShip()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	Attributes = CreateDefaultSubobject<UAsteroidsAttributeSet>(TEXT("Attributes"));

	// Collect all FXSystemComponents with tag "Engine"
	for (UActorComponent* Component : GetComponentsByTag(UFXSystemComponent::StaticClass(), TEXT("Engine")))
	{
		Engines.Add(Cast<UFXSystemComponent>(Component));
	}

	// Set some basic settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AAIShip::BeginPlay()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddUObject(this, &AAIShip::HealthChanged);

	InitializeAttributes();
	GiveAbilities();

	Super::BeginPlay();
}

// Called every frame
void AAIShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FaceTargetDirection(DeltaTime);
}

UAbilitySystemComponent* AAIShip::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAIShip::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AAIShip::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UAsteroidsGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
		}
	}
}

void AAIShip::HealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0)
	{
		Die();
	}
}

void AAIShip::FaceTargetDirection(float DeltaTime)
{
	// Get the manual attack abilities used by this enemy
	const FGameplayTagContainer AttackTags = FGameplayTagContainer(FAsteroidsGameplayTags::Get().Ability_Attack);
	TArray<FGameplayAbilitySpec*> AttackAbilities;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AttackTags, AttackAbilities);

	// Loop over them, get the first one as a subclass of UGameplayAbility_AttackManual
	const UGameplayAbility_AttackManual* AttackAbility = nullptr;
	for (const FGameplayAbilitySpec* Ability : AttackAbilities)
	{
		AttackAbility = Cast<UGameplayAbility_AttackManual>(Ability->Ability);
		if (AttackAbility)
		{
			break;
		}
	}

	// Calculate where we have to aim
	const AActor* Target = GetController<AAsteroidsAIController>()->GetFocusActor();
	if (Target)
	{
		FVector TargetLocation = Target->GetActorLocation();
		if (AttackAbility != nullptr)
		{
			const float ProjectileSpeed = AttackAbility->GetProjectileClass()->GetDefaultObject<AProjectile>()->GetInitialSpeed();
			TargetLocation = CalculateLeadLocation(GetActorLocation(), Target->GetActorLocation(), Target->GetVelocity(), ProjectileSpeed);
		}

		MovementComponent->RotateTowardsLocation(TargetLocation);
	}
}

void AAIShip::Die()
{
	AbilitySystemComponent->AddLooseGameplayTag(FAsteroidsGameplayTags::Get().State_Dead);

	const FVector Location = GetActorLocation();
	GetWorld()->SpawnActor(LootItemClass, &Location);

	Explode();
	Destroy();
}