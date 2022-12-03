// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "EnemyBase.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "Asteroids/Components/AIMovementComponent.h"
#include "Asteroids/Gameplay/AttributeSetBase.h"
#include "Asteroids/Gameplay/GameplayAbilityBase.h"
#include "Asteroids/Gameplay/GameplayAbility_AttackManual.h"
#include "Asteroids/Items/ItemProjectile.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);

	ExplosionNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ExplosionEffect"));
	ExplosionNiagaraComponent->SetupAttachment(Mesh);
	ExplosionAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound"));
	ExplosionAudioComponent->SetupAttachment(ExplosionNiagaraComponent);

	MovementComp = CreateDefaultSubobject<UAIMovementComponent>(TEXT("MovementComp"));

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	Attributes = CreateDefaultSubobject<UAttributeSetBase>(TEXT("Attributes"));

	for (UActorComponent* Component : GetComponentsByTag(UFXSystemComponent::StaticClass(), TEXT("Engine")))
	{
		Engines.Add(Cast<UFXSystemComponent>(Component));
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FaceTargetDirection(DeltaTime);
}

void AEnemyBase::HitByProjectile_Implementation(APawn* ProjectileInstigator,
	TSubclassOf<UGameplayEffect> ProjectileEffect)
{
	if (ProjectileEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(ProjectileInstigator);
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ProjectileEffect, 1,
			EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEnemyBase::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1,
			EffectContext);

		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AEnemyBase::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbilityBase>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1,
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	ExplosionNiagaraComponent->OnSystemFinished.AddDynamic(this, &AEnemyBase::OnExplosionFinished);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddUObject(this,
		&AEnemyBase::HealthChanged);

	InitializeAttributes();
	GiveAbilities();

	Super::BeginPlay();
}

void AEnemyBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0)
	{
		Die();
	}
}

void AEnemyBase::FaceTargetDirection(float DeltaTime)
{
	// Get the manual attack abilities used by this enemy
	const FGameplayTagContainer AttackTags = FGameplayTagContainer(
		FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Manual")));
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
	FVector TargetLocation = Target->GetActorLocation();
	if (AttackAbility != nullptr)
	{
		const float ProjectileSpeed = AttackAbility->GetProjectileClass()->GetDefaultObject<AItemProjectile>()->
		                                             GetInitialSpeed();
		TargetLocation = CalculateLeadLocation(GetActorLocation(), Target->GetActorLocation(),
			Target->GetVelocity(), ProjectileSpeed);
	}

	// Now we have our base values
	const FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = (TargetLocation - GetActorLocation()).Rotation();

	// Add some roll to the turn
	TargetRotation.Roll = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);

	// For the actual turn, just add the direction to the existing yaw
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TurnSpeed);
	// Clamp roll so we don't overshoot
	NewRotation.Roll = FMath::Clamp(NewRotation.Roll, -RollLimit, RollLimit);
	// Cancel any pitch caused by physics
	NewRotation.Pitch = 0;

	// Normalize and set to Mesh
	SetActorRotation(NewRotation);
}

void AEnemyBase::Die()
{
	AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));

	MovementComp->Deactivate();
	Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetVisibility(false, true);

	ExplosionNiagaraComponent->SetVisibility(true);
	ExplosionNiagaraComponent->ActivateSystem();
	ExplosionAudioComponent->Play();
}

void AEnemyBase::OnExplosionFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}
