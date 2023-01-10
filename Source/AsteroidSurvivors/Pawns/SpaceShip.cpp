// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "SpaceShip.h"

#include "AbilitySystem/AsteroidsGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Components/AsteroidsMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameplayEffectTypes.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "System/AsteroidsGameMode.h"

// Sets default values
ASpaceShip::ASpaceShip()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MovementComponent = CreateDefaultSubobject<UAsteroidsMovementComponent>(TEXT("MovementComponent"));

	// Setup components
	RootComponent = CapsuleComponent;
	MeshComponent->SetupAttachment(RootComponent);

	// Setup some basic settings for all spaceships
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetNotifyRigidBodyCollision(true);
	CapsuleComponent->SetCollisionProfileName(FName(TEXT("Pawn")));
	CapsuleComponent->GetBodyInstance()->bLockZTranslation = true;
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();

	// Add event listeners
	CapsuleComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* ASpaceShip::GetAbilitySystemComponent() const
{
	// We need the ASC of subclasses, so just return nullptr here
	return nullptr;
}

void ASpaceShip::HitByProjectile_Implementation(APawn* ProjectileInstigator, FGameplayEffectSpecHandle EffectSpec)
{
	if (GetAbilitySystemComponent() && EffectSpec.IsValid())
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		const FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());

		if (ActiveEffectHandle.WasSuccessfullyApplied())
		{
			TakeDamage(
				FMath::Abs(EffectSpec.Data.Get()->GetSetByCallerMagnitude(FAsteroidsGameplayTags::Get().Effect_Damage)),
				FPointDamageEvent(),
				ProjectileInstigator->GetController(),
				ProjectileInstigator
			);
		}
	}
}

void ASpaceShip::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const AAsteroidsGameMode* GameMode = Cast<AAsteroidsGameMode>(UGameplayStatics::GetGameMode(this));

	const double Damage = FMath::RoundToZero(NormalImpulse.Size() / 10000.f);
	if (Damage <= 0.f)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(OtherActor);

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameMode->GetCollisionGameplayEffectClass(), 1, EffectContext);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(FAsteroidsGameplayTags::Get().Effect_Damage, -Damage);
	if (SpecHandle.IsValid())
	{
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		AController* OtherController = IsValid(OtherActor) ? OtherActor->GetInstigatorController() : nullptr;
		TakeDamage(Damage, FPointDamageEvent(), OtherController, OtherActor);
	}
}

void ASpaceShip::Explode_Implementation()
{
	RootComponent->SetVisibility(false, true);
	MovementComponent->Deactivate();
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionParticleEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSoundEffect, GetActorLocation());
}